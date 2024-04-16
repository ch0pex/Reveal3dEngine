/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_pso.hpp
 * @version 1.0
 * @date 02/04/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "dx_common.hpp"
#include "dx_root_signature.hpp"

namespace reveal3d::graphics::dx {

class GraphicsPso {
public:
    GraphicsPso();

    void SetPsoDesc(D3D12_GRAPHICS_PIPELINE_STATE_DESC &desc);

    void SetInputLayout(const D3D12_INPUT_ELEMENT_DESC* inputLayout, u32 count);
    void SetRootSignature(RootSignature& rootSignature);
    void SetShaders(ID3DBlob* vs, ID3DBlob* ps);
    void SetRasterizerCullMode (const D3D12_CULL_MODE mode);
    void SetBlendState(const D3D12_BLEND_DESC &blendDesc);
    void SetDepthStencil(const D3D12_DEPTH_STENCIL_DESC& dsDesc);
    void SetSampleMask(const u32 mask);
    void SetPrimitive(const D3D12_PRIMITIVE_TOPOLOGY_TYPE primitive);
    void SetNumRenderTargets(const u32 number);
    void SetRtvFormats(const u32 index, const DXGI_FORMAT format);
    void SetSampleDescCount(const u32 sampleCount);
    void SetDSVFormat(const DXGI_FORMAT dsvFormat);

    void Finalize(ID3D12Device *device);
    INLINE ID3D12PipelineState* Get() { return pipelineState_.Get(); }


private:
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    RootSignature* rootSignature_;
    ComPtr<ID3D12PipelineState> pipelineState_;
};

}

