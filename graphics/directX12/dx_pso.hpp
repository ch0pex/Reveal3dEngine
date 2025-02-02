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

#include <d3dcompiler.h>


#include "dx_adapter.hpp"
#include "dx_root_signature.hpp"
#include "dx_shader.hpp"
#include "utils/dx_debug.hpp"

namespace reveal3d::graphics::dx12 {

class GraphicsPso {
public:
  using descriptor = D3D12_GRAPHICS_PIPELINE_STATE_DESC;

  explicit GraphicsPso(descriptor const& desc) {
    adapter.device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pipeline_state_)) >> utils::DxCheck;
  };
  [[nodiscard]] ID3D12PipelineState* get() const { return pipeline_state_.Get(); }

private:
  ComPtr<ID3D12PipelineState> pipeline_state_;
};

} // namespace reveal3d::graphics::dx12
