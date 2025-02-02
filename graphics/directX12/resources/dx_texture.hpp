/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_texture.hpp
 * @version 1.0
 * @date 10/11/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "dx_buffer.hpp"
#include "dx_descriptor_heap.hpp"
#include "dx_descriptor_heap_type.hpp"
#include "dx_descriptors.hpp"
#include "graphics/directX12/utils/dx_defaults.hpp"

namespace reveal3d::graphics::dx12 {

class Texture {
public:
  using init_info = Buffer::init_info;
  using heap_type = DescriptorHeap<HeapType::Srv>;

  constexpr static u32 max_mips {14};

  Texture(DescriptorHandle const& handle, init_info const& info) : buffer_(defaults::texture2d()), srv_(handle) { }

  Texture(Texture&& other) noexcept : buffer_(std::move(other.buffer_)), srv_(std::move(other.srv_)) {
    // other.reset();
  }

  [[nodiscard]] auto const& handle() const { return srv_; }

  [[nodiscard]] auto resource() const { return buffer_.resource(); }

private:
  Buffer buffer_;
  DescriptorHandle srv_;
};


} // namespace reveal3d::graphics::dx12
