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
#include "dx_buffer.hpp"
#include "dx_descriptor_heap.hpp"

namespace reveal3d::graphics::dx12 {

namespace defaults {

constexpr D3D12_DEPTH_STENCIL_VIEW_DESC dsv_desc = {
  .Format        = DXGI_FORMAT_D24_UNORM_S8_UINT,
  .ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D,
  .Flags         = D3D12_DSV_FLAG_NONE,
  .Texture2D     = {.MipSlice = 0}
};

constexpr D3D12_CLEAR_VALUE clear_value = { //
  .Format = DXGI_FORMAT_D24_UNORM_S8_UINT,
  .DepthStencil =
      {
        .Depth   = 1.0F,
        .Stencil = 0,
      }
};

inline auto depth_buffer_desc(window::Resolution const res) {
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


inline auto depth_buffer_info(window::Resolution const res) {
  return Buffer::InitInfo {
    .res_desc    = depth_buffer_desc(res),
    .res_state   = D3D12_RESOURCE_STATE_DEPTH_WRITE,
    .clear_value = clear_value,
  };
}

} // namespace defaults

/**
 * @class DepthBuffer
 * This class creates a gpu resource that is used as a
 * depth stencil buffer
 */
class DepthBuffer {
public:
  // *** Constructors ***
  /**
   * @param res Depth buffer resolution
   * @param handle
   * @param heap Depth stencil descriptor heap where descriptor will be allocated
   */
  explicit DepthBuffer(DescriptorHandle const& handle, window::Resolution const res) :
    dsv_(handle), buff_(defaults::depth_buffer_info(res)) {
    createView();
  }

  explicit DepthBuffer(DescriptorHandle const& handle, Buffer::InitInfo const& info) : dsv_(handle), buff_(info) {
    createView();
  }

  [[nodiscard]] auto const& handle() const { return dsv_; }

  // [[nodiscard]] auto& srv() const { return dsv_.cpu; }

  [[nodiscard]] auto resource() const -> ID3D12Resource* { return buff_.resource(); }

  void resize(window::Resolution const res) {
    buff_ = Buffer {defaults::depth_buffer_info(res)};
    createView();
  }

private:
  void createView() const { adapter.device->CreateDepthStencilView(buff_.resource(), &defaults::dsv_desc, dsv_.cpu); }

  DescriptorHandle dsv_;
  Buffer buff_; // TODO change this for Texture
};

} // namespace reveal3d::graphics::dx12
