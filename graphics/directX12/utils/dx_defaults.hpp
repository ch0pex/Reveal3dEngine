/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_defaults.hpp
 * @version 1.0
 * @date 23/01/2025
 * @brief D
 *
 */

#pragma once

#include "graphics/directX12/resources/dx_buffer.hpp"
#include "window/window_info.hpp"

#include <d3d12.h>


namespace reveal3d::graphics::dx12::defaults {

constexpr D3D12_RENDER_TARGET_VIEW_DESC rtv_descriptor = {
  .Format = DXGI_FORMAT_R8G8B8A8_UNORM, .ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D
};

constexpr D3D12_COMMAND_QUEUE_DESC queue_desc {
  .Type     = D3D12_COMMAND_LIST_TYPE_DIRECT,
  .Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
  .Flags    = D3D12_COMMAND_QUEUE_FLAG_NONE,
  .NodeMask = 0
};

constexpr D3D12_DEPTH_STENCIL_VIEW_DESC dsv_desc = {
  .Format        = DXGI_FORMAT_D24_UNORM_S8_UINT,
  .ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D,
  .Flags         = D3D12_DSV_FLAG_NONE,
  .Texture2D     = {.MipSlice = 0}
};

constexpr D3D12_CLEAR_VALUE clear_value =
    { //
      .Format = DXGI_FORMAT_D24_UNORM_S8_UINT,
      .DepthStencil =
          {
            .Depth   = 1.0F,
            .Stencil = 0,
          }
};

constexpr auto depth_buffer_desc(window::Resolution const res) {
  return D3D12_RESOURCE_DESC {
    .Dimension        = D3D12_RESOURCE_DIMENSION_TEXTURE2D,
    .Alignment        = 0,
    .Width            = res.width,
    .Height           = res.height,
    .DepthOrArraySize = 1,
    .MipLevels        = 1,
    .Format           = DXGI_FORMAT_R24G8_TYPELESS,
    .SampleDesc =
        {
          .Count   = 1,
          .Quality = 0,
        },
    .Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN,
    .Flags  = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL,
  };
}


constexpr auto depth_buffer_info(window::Resolution const res) {
  return BufferDescriptor {
    .res_desc    = depth_buffer_desc(res),
    .res_state   = D3D12_RESOURCE_STATE_DEPTH_WRITE,
    .clear_value = clear_value,
  };
}

template<typename Policy = policy::Hard>
constexpr auto buffer1d(u64 const width, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN) {
  return BufferDescriptor {
    .res_desc  = CD3DX12_RESOURCE_DESC::Buffer(width),
    .res_state = D3D12_RESOURCE_STATE_COMMON,
    .heap_properties {CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT)},
    .clear_value {std::nullopt},
  };
}

constexpr auto texture2d(
    window::Resolution const resolution, DXGI_FORMAT const format,
    D3D12_RESOURCE_FLAGS const flags = D3D12_RESOURCE_FLAG_NONE
) {
  return Buffer<>::init_info {
    .res_desc =
        {
          .Dimension        = D3D12_RESOURCE_DIMENSION_TEXTURE2D,
          .Alignment        = 0,
          .Width            = resolution.width,
          .Height           = resolution.height,
          .DepthOrArraySize = 1,
          .MipLevels        = 1,
          .Format           = format,
          .SampleDesc       = {1, 0},
          .Layout           = D3D12_TEXTURE_LAYOUT_UNKNOWN,
          .Flags            = flags,
        },
    .res_state       = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
    .heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
    .clear_value     = clear_value,
  };
}

constexpr auto pipeline_desc = [] {
  D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {
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

} // namespace reveal3d::graphics::dx12::defaults
