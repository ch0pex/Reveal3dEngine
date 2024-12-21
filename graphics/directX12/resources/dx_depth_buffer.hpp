/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_depth_buffer.hpp
 * @version 1.0
 * @date 24/11/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "../dx_adapter.hpp"
#include "../dx_common.hpp"
#include "dx_buffer.hpp"
#include "dx_descriptor_heap.hpp"

namespace reveal3d::graphics::dx12 {

class DepthBuffer {
public:
  explicit DepthBuffer(window::Resolution const res, DescriptorHeap& heap) : handle_(heap.alloc()) { init(res); }

  auto static constexpr default_desc = [](u64 const width, u32 const height) {
    return D3D12_RESOURCE_DESC {
      .Dimension        = D3D12_RESOURCE_DIMENSION_TEXTURE2D,
      .Alignment        = 0,
      .Width            = width,
      .Height           = height,
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
  };

  auto static constexpr default_opt_clear = []() {
    return D3D12_CLEAR_VALUE {
      .Format = DXGI_FORMAT_D24_UNORM_S8_UINT,
      .DepthStencil =
          {
            .Depth   = 1.0f,
            .Stencil = 0,
          }
    };
  };

  [[nodiscard]] constexpr auto const& cpu() const { return handle_.cpu; }

  void resize(window::Resolution const res, Heaps& heaps) {
    buff_.release();
    init(res);
  }

private:
  void init(window::Resolution const res) {
    Buffer::InitInfo const buf_info {
      .res_desc    = default_desc(res.width, res.height),
      .res_state   = D3D12_RESOURCE_STATE_DEPTH_WRITE,
      .clear_value = default_opt_clear(),
    };

    buff_.init(buf_info);

    constexpr D3D12_DEPTH_STENCIL_VIEW_DESC dsv_desc = {
      .Format        = DXGI_FORMAT_D24_UNORM_S8_UINT,
      .ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D,
      .Flags         = D3D12_DSV_FLAG_NONE,
      .Texture2D     = {.MipSlice = 0}
    };

    adapter.device->CreateDepthStencilView(buff_.resource(), &dsv_desc, handle_.cpu);
  }

  Buffer buff_;
  DescriptorHandle handle_;
};

} // namespace reveal3d::graphics::dx12
