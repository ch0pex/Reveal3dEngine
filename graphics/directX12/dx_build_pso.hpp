/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_build_pso.hpp
 * @version 1.0
 * @date 19/01/2025
 * @brief Short description
 *
 * Longer description
 */

#pragma once
#include "dx_pso.hpp"

namespace reveal3d::graphics::dx12 {

namespace detail {

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

} // namespace detail

inline GraphicsPso build_opaque(ID3D12RootSignature* root_signature) {
  auto const [vs, ps] = compile_shader(
      L"graphics/shaders/hlsl/OpaqueShader.hlsl", //
      L"graphics/shaders/hlsl/OpaqueShader.hlsl"
  );

  D3D12_INPUT_ELEMENT_DESC input_element_desc[] = {
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
    {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
    {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
  };

  GraphicsPso::descriptor desc_opaque = defaults::pipeline_desc();
  desc_opaque.pRootSignature          = root_signature;
  desc_opaque.VS                      = CD3DX12_SHADER_BYTECODE(vs.Get());
  desc_opaque.PS                      = CD3DX12_SHADER_BYTECODE(ps.Get());
  desc_opaque.InputLayout             = {input_element_desc, _countof(input_element_desc)};

  return GraphicsPso {desc_opaque};
}

inline GraphicsPso build_flat(ID3D12RootSignature* root_signature) {
  auto const [vs, ps] = compile_shader(
      L"graphics/shaders/hlsl/FlatShader.hlsl", //
      L"graphics/shaders/hlsl/FlatShader.hlsl"
  );


  D3D12_INPUT_ELEMENT_DESC flat_elements_desc[] = {
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
    {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
  };

  GraphicsPso::descriptor desc_flat = defaults::pipeline_desc();
  desc_flat.pRootSignature          = root_signature;
  desc_flat.VS                      = CD3DX12_SHADER_BYTECODE(vs.Get());
  desc_flat.PS                      = CD3DX12_SHADER_BYTECODE(ps.Get());
  desc_flat.BlendState              = detail::blend_desc;
  desc_flat.InputLayout             = {flat_elements_desc, _countof(flat_elements_desc)};

  return GraphicsPso {desc_flat};
}

inline GraphicsPso build_grid(ID3D12RootSignature* root_signature) {
  auto const [vs, ps] = compile_shader(
      L"graphics/shaders/hlsl/GridShader.hlsl", //
      L"graphics/shaders/hlsl/GridShader.hlsl"
  );

  D3D12_INPUT_ELEMENT_DESC flat_elements_desc[] = {
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
    {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
  };

  GraphicsPso::descriptor desc_grid = defaults::pipeline_desc();
  desc_grid.pRootSignature          = root_signature;
  desc_grid.VS                      = CD3DX12_SHADER_BYTECODE(vs.Get());
  desc_grid.PS                      = CD3DX12_SHADER_BYTECODE(ps.Get());
  desc_grid.BlendState              = detail::blend_desc;
  desc_grid.InputLayout             = {flat_elements_desc, _countof(flat_elements_desc)};

  return GraphicsPso {desc_grid};
}

} // namespace reveal3d::graphics::dx12
