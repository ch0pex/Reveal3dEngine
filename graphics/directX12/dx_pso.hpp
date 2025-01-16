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
#include "utils/dx_debug.hpp"

namespace reveal3d::graphics::dx12 {

class GraphicsPso {
public:
  using descriptor = D3D12_GRAPHICS_PIPELINE_STATE_DESC;

  explicit GraphicsPso(descriptor const& desc) {
    adapter.device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pipeline_state_)) >> utils::DxCheck;
  };
  [[nodiscard]] ID3D12PipelineState* get() const { return pipeline_state_.Get(); }

  constexpr static auto default_desc = [] {
    descriptor desc = {
      .SampleMask            = std::numeric_limits<u32>::max(),
      .RasterizerState       = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT),
      .DepthStencilState     = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT),
      .PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
      .NumRenderTargets      = 1,
      .DSVFormat             = DXGI_FORMAT_D24_UNORM_S8_UINT,
      .SampleDesc            = {.Count = 1},
    };
    desc.BlendState               = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    desc.RTVFormats[0]            = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
    return desc;
  };

private:
  ComPtr<ID3D12PipelineState> pipeline_state_;
};

struct ShaderBinary {
  ComPtr<ID3DBlob> vs;
  ComPtr<ID3DBlob> ps;
};

inline ShaderBinary compile_shader(std::wstring_view const vertex_file, std::wstring_view const pixel_file) {
  ComPtr<ID3DBlob> vertex_shader;
  ComPtr<ID3DBlob> pixel_shader;
  ComPtr<ID3DBlob> errors;

#if defined(_DEBUG)
  // Enable better Shader debugging with the graphics debugging tools.
  UINT compile_flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
  UINT compile_flags = 0;
#endif
  HRESULT hr = S_OK;

  // TODO Config file for assets path
  hr = D3DCompileFromFile(
      absolute(vertex_file).c_str(), nullptr, nullptr, "VS", "vs_5_0", compile_flags, 0, &vertex_shader, &errors
  );
  if (errors != nullptr)
    logger(LogInfo) << static_cast<char*>(errors->GetBufferPointer());

  hr >> utils::DxCheck;
  hr = D3DCompileFromFile(
      absolute(pixel_file).c_str(), nullptr, nullptr, "PS", "ps_5_0", compile_flags, 0, &pixel_shader, &errors
  );
  if (errors != nullptr)
    logger(LogInfo) << static_cast<char*>(errors->GetBufferPointer());
  hr >> utils::DxCheck;

  return {vertex_shader, pixel_shader};
}

} // namespace reveal3d::graphics::dx12
