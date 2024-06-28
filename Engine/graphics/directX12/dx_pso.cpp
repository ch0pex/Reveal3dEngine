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

GraphicsPso::GraphicsPso() {}

void GraphicsPso::SetPsoDesc(D3D12_GRAPHICS_PIPELINE_STATE_DESC &desc) {
    psoDesc = desc;
}

void GraphicsPso::SetInputLayout(const D3D12_INPUT_ELEMENT_DESC* inputLayout, u32 count) {
   psoDesc.InputLayout = { inputLayout, count };
}

void GraphicsPso::SetRootSignature(RootSignature &rootSignature) {
    psoDesc.pRootSignature = rootSignature.Get();
}

void GraphicsPso::SetShaders(ID3DBlob *vs, ID3DBlob *ps) {
    psoDesc.VS = CD3DX12_SHADER_BYTECODE(vs);
    psoDesc.PS = CD3DX12_SHADER_BYTECODE(ps);
}

void GraphicsPso::SetRasterizerCullMode(const D3D12_CULL_MODE mode) {
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.RasterizerState.CullMode = mode;
}

void GraphicsPso::SetBlendState(const D3D12_BLEND_DESC &blendDesc) {
    psoDesc.BlendState = blendDesc;
}

void GraphicsPso::SetDepthStencil(const D3D12_DEPTH_STENCIL_DESC& dsDesc) {
    psoDesc.DepthStencilState = dsDesc;
}

void GraphicsPso::SetSampleMask(const u32 mask) {
    psoDesc.SampleMask = mask;
}

void GraphicsPso::SetPrimitive(const D3D12_PRIMITIVE_TOPOLOGY_TYPE primitive) {
    psoDesc.PrimitiveTopologyType = primitive;
}

void GraphicsPso::SetNumRenderTargets(const u32 number) {
    psoDesc.NumRenderTargets = number;
}

void GraphicsPso::SetRtvFormats(const u32 index, const DXGI_FORMAT format) {
    psoDesc.RTVFormats[index] = format;
}

void GraphicsPso::SetSampleDescCount(const u32 sampleCount) {
    psoDesc.SampleDesc.Count = sampleCount;
}

void GraphicsPso::SetDSVFormat(const DXGI_FORMAT dsvFormat) {
    psoDesc.DSVFormat = dsvFormat;
}

void GraphicsPso::Finalize(ID3D12Device *device) {
    device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState_)) >> utl::DxCheck;
}

}