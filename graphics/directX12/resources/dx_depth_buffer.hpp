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

#include "dx_buffer.hpp"
#include "dx_descriptor_heap.hpp"
#include "graphics/directX12/dx_adapter.hpp"
#include "utils/dx_defaults.hpp"

namespace reveal3d::graphics::dx12 {

/**
 * @class DepthBuffer
 * This class creates a gpu resource that is used as a
 * depth stencil buffer
 */
class DepthBuffer {
public:
  // *** Constructors ***
  explicit DepthBuffer(DescriptorHandle const& handle, window::Resolution const res) :
    dsv_(handle), buff_(defaults::depth_buffer_info(res)) {
    createView();
  }

  explicit DepthBuffer(DescriptorHandle const& handle, Buffer::InitInfo const& info) : dsv_(handle), buff_(info) {
    createView();
  }

  [[nodiscard]] auto const& handle() const { return dsv_; }

  [[nodiscard]] auto resource() const -> ID3D12Resource* { return buff_.resource(); }

  void resize(window::Resolution const res) {
    buff_ = Buffer {defaults::depth_buffer_info(res)};
    createView();
  }

  void release() { buff_.release(); }

private:
  void createView() const { adapter.device->CreateDepthStencilView(buff_.resource(), &defaults::dsv_desc, dsv_.cpu); }

  DescriptorHandle dsv_;
  Buffer buff_; // TODO change this for Texture
};

} // namespace reveal3d::graphics::dx12
