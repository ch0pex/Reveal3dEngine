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

namespace reveal3d::graphics::dx12 {

class GraphicsPso {
public:
  GraphicsPso();

  void setPsoDesc(D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc);
  void setInputLayout(const D3D12_INPUT_ELEMENT_DESC* input_layout, u32 count);
  void setRootSignature(RootSignature& root_signature);
  void setShaders(ID3DBlob* vs, ID3DBlob* ps);
  void setRasterizerCullMode(const D3D12_CULL_MODE mode);
  void setBlendState(const D3D12_BLEND_DESC& blend_desc);
  void setDepthStencil(const D3D12_DEPTH_STENCIL_DESC& ds_desc);
  void setSampleMask(const u32 mask);
  void setPrimitive(const D3D12_PRIMITIVE_TOPOLOGY_TYPE primitive);
  void setNumRenderTargets(const u32 number);
  void setRtvFormats(const u32 index, const DXGI_FORMAT format);
  void setSampleDescCount(const u32 sample_count);
  void setDsvFormat(const DXGI_FORMAT dsv_format);

  void finalize(ID3D12Device* device);
  ID3D12PipelineState* get() const { return pipeline_state_.Get(); }


private:
  D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc_ = {};
  ComPtr<ID3D12PipelineState> pipeline_state_;
};


} // namespace reveal3d::graphics::dx12
