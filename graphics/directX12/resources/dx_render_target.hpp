/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_render_target.hpp
 * @version 1.0
 * @date 29/01/2025
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "graphics/directX12/resources/dx_descriptor_heap.hpp"


namespace reveal3d::graphics::dx12 {

class RenderTarget {
public:
  using init_info = D3D12_RENDER_TARGET_VIEW_DESC;
  using heap_type = DescriptorHeap<HeapType::Rtv>;

  RenderTarget() = default;

  RenderTarget(DescriptorHandle const& handle, init_info const& desc, ID3D12Resource* buffer) :
    buf_(buffer), rtv_(handle) {
    adapter.device->CreateRenderTargetView(buf_, &desc, rtv_.cpu);
  }

  ~RenderTarget() { release(); }

  RenderTarget& operator=(RenderTarget&& other) noexcept {
    buf_       = other.buf_;
    rtv_       = other.rtv_;
    other.buf_ = nullptr;
    other.rtv_ = {};

    return *this;
  }

  [[nodiscard]] auto const& handle() const { return rtv_; }

  [[nodiscard]] auto resource() const { return buf_; }

  void release() { dx12::release(buf_); }

private:
  ID3D12Resource* buf_ {};
  DescriptorHandle rtv_ {};
};

} // namespace reveal3d::graphics::dx12
