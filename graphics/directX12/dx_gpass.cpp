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
#include "dx_build_pso.hpp"

namespace reveal3d::graphics::dx12 {

auto constexpr build_root = [](RootSignature& root) {
  root[0].InitAsConstantBufferView(0);
  root[1].InitAsConstantBufferView(1);
  root[2].InitAsConstantBufferView(2);
  root[3].InitAsShaderResourceView(3);
  root.finalize();
};

Gpass::Gpass(window::Resolution const resolution, Heaps& heaps) :
  depth_buffer_(heaps.dsv.alloc<DepthBuffer>(resolution)), //
  root_signatures_ {RootSignature {4, build_root}, RootSignature {4, build_root}, RootSignature {4, build_root}},
  pipeline_states_ {
    build_flat(root_signatures_.at(static_cast<u8>(EngineShader::Flat)).get()), //
    build_opaque(root_signatures_.at(static_cast<u8>(EngineShader::Opaque)).get()), //
    build_grid(root_signatures_.at(static_cast<u8>(EngineShader::Grid)).get())
  } { }


void Gpass::render(ID3D12GraphicsCommandList* command_list, FrameResource const& frame_resource) {
  curr_root_signature_ = nullptr;
  curr_pipeline_state_ = nullptr;

  for (auto& render_element: render_elements_) {

    auto geometry  = render_element.entity().component<core::Geometry>();
    auto transform = render_element.entity().component<core::Transform>();

    for (auto& [vtx_pos, idx_pos, idx_count, _, shader]: std::views::filter(geometry.subMeshes(), render::is_visible)) {
      if (curr_root_signature_ != root_signatures_.at(shader).get()) {
        curr_root_signature_ = root_signatures_.at(shader).get();
        command_list->SetGraphicsRootSignature(curr_root_signature_);
      }

      if (curr_pipeline_state_ != pipeline_states_.at(shader).get()) {
        curr_pipeline_state_ = pipeline_states_.at(shader).get();
        command_list->SetPipelineState(curr_pipeline_state_);
      }

      /// This is the most expensive way to set RootParameters limit 64 DWORDS, currently using 6
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
      command_list->DrawIndexedInstanced(idx_count, 1, idx_pos, vtx_pos, 0);
    }
  }
  if (config::scene.showGrid) {
    drawWorldGrid(command_list, frame_resource);
  }
}

void Gpass::drawWorldGrid(ID3D12GraphicsCommandList* command_list, FrameResource const& frame_resource) const {
  command_list->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  command_list->SetGraphicsRootSignature(root_signatures_[static_cast<u8>(EngineShader::Grid)].get());
  command_list->SetPipelineState(pipeline_states_[static_cast<u8>(EngineShader::Grid)].get());
  command_list->SetGraphicsRootConstantBufferView(2, frame_resource.pass_buffer.gpuStart());
  command_list->DrawInstanced(6, 1, 0, 0);
}

// NOTE change geometry to mesh when content module update
void Gpass::addRenderElement(core::Geometry geo, Commands const& cmd_mng) {
  render_elements_.emplace_back(geo, cmd_mng.list());
}


} // namespace reveal3d::graphics::dx12
