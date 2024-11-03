/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_pso.cpp
 * @version 1.0
 * @date 02/04/2024
 * @brief Short description
 *
 * Longer description
 */

#include "dx_pso.hpp"

namespace reveal3d::graphics::dx12 {

GraphicsPso::GraphicsPso() { }

void GraphicsPso::setPsoDesc(D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc) { pso_desc_ = desc; }

void GraphicsPso::setInputLayout(const D3D12_INPUT_ELEMENT_DESC* input_layout, u32 count) {
  pso_desc_.InputLayout = {input_layout, count};
}

void GraphicsPso::setRootSignature(RootSignature& root_signature) { pso_desc_.pRootSignature = root_signature.get(); }

void GraphicsPso::setShaders(ID3DBlob* vs, ID3DBlob* ps) {
  pso_desc_.VS = CD3DX12_SHADER_BYTECODE(vs);
  pso_desc_.PS = CD3DX12_SHADER_BYTECODE(ps);
}

void GraphicsPso::setRasterizerCullMode(const D3D12_CULL_MODE mode) {
  pso_desc_.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
  pso_desc_.RasterizerState.CullMode = mode;
}

void GraphicsPso::setBlendState(const D3D12_BLEND_DESC& blend_desc) { pso_desc_.BlendState = blend_desc; }

void GraphicsPso::setDepthStencil(const D3D12_DEPTH_STENCIL_DESC& ds_desc) { pso_desc_.DepthStencilState = ds_desc; }

void GraphicsPso::setSampleMask(const u32 mask) { pso_desc_.SampleMask = mask; }

void GraphicsPso::setPrimitive(const D3D12_PRIMITIVE_TOPOLOGY_TYPE primitive) {
  pso_desc_.PrimitiveTopologyType = primitive;
}

void GraphicsPso::setNumRenderTargets(const u32 number) { pso_desc_.NumRenderTargets = number; }

void GraphicsPso::setRtvFormats(const u32 index, const DXGI_FORMAT format) { pso_desc_.RTVFormats[index] = format; }

void GraphicsPso::setSampleDescCount(const u32 sample_count) { pso_desc_.SampleDesc.Count = sample_count; }

void GraphicsPso::setDsvFormat(const DXGI_FORMAT dsv_format) { pso_desc_.DSVFormat = dsv_format; }

void GraphicsPso::finalize(ID3D12Device* device) {
  device->CreateGraphicsPipelineState(&pso_desc_, IID_PPV_ARGS(&pipeline_state_)) >> utl::DxCheck;
}

} // namespace reveal3d::graphics::dx12
