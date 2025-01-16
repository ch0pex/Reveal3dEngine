/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_gpass.cpp
 * @version 1.0
 * @date 21/03/2024
 * @brief Config engine structs
 *
 * All this structs can be used to configure the system
 */

#include "dx_gpass.hpp"

#include "core/components/geometry.hpp"
#include "core/components/transform.hpp"

#include <d3dcompiler.h>

namespace reveal3d::graphics::dx12 {

constexpr D3D12_RENDER_TARGET_BLEND_DESC transparency_blend_desc {
  .BlendEnable           = 1,
  .LogicOpEnable         = 0,
  .SrcBlend              = D3D12_BLEND_SRC_ALPHA,
  .DestBlend             = D3D12_BLEND_INV_SRC_ALPHA,
  .BlendOp               = D3D12_BLEND_OP_ADD,
  .SrcBlendAlpha         = D3D12_BLEND_ONE,
  .DestBlendAlpha        = D3D12_BLEND_ZERO,
  .BlendOpAlpha          = D3D12_BLEND_OP_ADD,
  .LogicOp               = D3D12_LOGIC_OP_NOOP,
  .RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL,
};

constexpr D3D12_BLEND_DESC blend_desc = {.RenderTarget = transparency_blend_desc};

Gpass::Gpass(window::Resolution const resolution, Heaps& heaps) :
  depth_buffer_(heaps.dsv.alloc<DepthBuffer>(resolution)), pipeline_states_() {
  root_signatures_.at(Shader::Opaque).reset(4);
  root_signatures_.at(Shader::Flat).reset(4);
  root_signatures_.at(Shader::Grid).reset(4);
  buildRoots();
  buildPsos();
}


void Gpass::render(ID3D12GraphicsCommandList* command_list, FrameResource const& frame_resource) {
  curr_root_signature_ = nullptr;
  curr_pipeline_state_ = nullptr;


  for (auto& render_element: render_elements_) {

    auto geometry  = render_element.entity().component<core::Geometry>();
    auto transform = render_element.entity().component<core::Transform>();

    for (auto& [vertex_pos, index_pos, index_count, visible, shader]: geometry.subMeshes()) {
      if (!visible)
        continue;

      if (curr_root_signature_ != root_signatures_.at(shader).get()) {
        curr_root_signature_ = root_signatures_.at(shader).get();
        command_list->SetGraphicsRootSignature(curr_root_signature_);
      }

      if (curr_pipeline_state_ != pipeline_states_.at(shader)->get()) {
        curr_pipeline_state_ = pipeline_states_.at(shader)->get();
        command_list->SetPipelineState(curr_pipeline_state_);
      }

      /// @note This is the most expensive way to set RootParameters limit 64 DWORDS, currently using 6
      command_list->SetGraphicsRootConstantBufferView(
          0, frame_resource.per_obj_buffer.gpuPos(id::index(transform.id()))
      ); // 2 DWORDS
      command_list->SetGraphicsRootConstantBufferView(
          1, frame_resource.mat_buffer.gpuPos(id::index(geometry.id()))
      ); // 2 DWORDS
      command_list->SetGraphicsRootConstantBufferView(2, frame_resource.pass_buffer.gpuStart()); // 2 DWORDS

      command_list->IASetVertexBuffers(0, 1, &render_element.vertices());
      command_list->IASetIndexBuffer(&render_element.indices());
      command_list->IASetPrimitiveTopology(render_element.topology());
      command_list->DrawIndexedInstanced(index_count, 1, index_pos, vertex_pos, 0);
    }
  }
  if (config::scene.showGrid) {
    drawWorldGrid(command_list, frame_resource);
  }
}

void Gpass::drawWorldGrid(ID3D12GraphicsCommandList* command_list, FrameResource const& frame_resource) const {
  command_list->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  command_list->SetGraphicsRootSignature(root_signatures_[Shader::Grid].get());
  command_list->SetPipelineState(pipeline_states_[Shader::Grid]->get());
  command_list->SetGraphicsRootConstantBufferView(2, frame_resource.pass_buffer.gpuStart());
  command_list->DrawInstanced(6, 1, 0, 0);
}

// NOTE change geometry to mesh when content module update
void Gpass::addRenderElement(core::Geometry geo, Commands const& cmd_mng) {
  render_elements_.emplace_back(geo, cmd_mng.list());
}

void Gpass::buildPsos() {

  ShaderBinary const opaque_shader_binary = compile_shader(
      L"graphics/shaders/hlsl/OpaqueShader.hlsl", //
      L"graphics/shaders/hlsl/OpaqueShader.hlsl"
  );
  ShaderBinary const grid_shader_binary = compile_shader(
      L"graphics/shaders/hlsl/GridShader.hlsl", //
      L"graphics/shaders/hlsl/GridShader.hlsl"
  );
  ShaderBinary const flat_shader_binary = compile_shader(
      L"graphics/shaders/hlsl/FlatShader.hlsl", //
      L"graphics/shaders/hlsl/FlatShader.hlsl"
  );

  D3D12_INPUT_ELEMENT_DESC input_element_desc[] = {
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
    {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
    {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
  };

  D3D12_INPUT_ELEMENT_DESC flat_elements_desc[] = {
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
    {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
  };

  GraphicsPso::descriptor desc_opaque = GraphicsPso::default_desc();
  desc_opaque.pRootSignature          = root_signatures_[Shader::Opaque].get();
  desc_opaque.VS                      = CD3DX12_SHADER_BYTECODE(opaque_shader_binary.vs.Get());
  desc_opaque.PS                      = CD3DX12_SHADER_BYTECODE(opaque_shader_binary.ps.Get());
  desc_opaque.InputLayout             = {input_element_desc, _countof(input_element_desc)};

  GraphicsPso::descriptor desc_flat = GraphicsPso::default_desc();
  desc_flat.pRootSignature          = root_signatures_[Shader::Opaque].get();
  desc_flat.VS                      = CD3DX12_SHADER_BYTECODE(flat_shader_binary.vs.Get());
  desc_flat.PS                      = CD3DX12_SHADER_BYTECODE(flat_shader_binary.ps.Get());
  desc_flat.BlendState              = blend_desc;
  desc_flat.InputLayout             = {flat_elements_desc, _countof(flat_elements_desc)};

  GraphicsPso::descriptor desc_grid = GraphicsPso::default_desc();
  desc_grid.pRootSignature          = root_signatures_[Shader::Grid].get();
  desc_grid.VS                      = CD3DX12_SHADER_BYTECODE(grid_shader_binary.vs.Get());
  desc_grid.PS                      = CD3DX12_SHADER_BYTECODE(grid_shader_binary.ps.Get());
  desc_grid.BlendState              = blend_desc;
  desc_grid.InputLayout             = {flat_elements_desc, _countof(flat_elements_desc)};

  pipeline_states_.at(Shader::Opaque).emplace(desc_opaque);
  // pipeline_states_.at(Shader::Flat).emplace(desc_flat);
  pipeline_states_.at(Shader::Grid).emplace(desc_grid);
}

void Gpass::buildRoots() {
  for (auto& root: root_signatures_) {
    root[0].InitAsConstantBufferView(0);
    root[1].InitAsConstantBufferView(1);
    root[2].InitAsConstantBufferView(2);
    root[3].InitAsShaderResourceView(3);
    root.finalize();
  }
}


} // namespace reveal3d::graphics::dx12
