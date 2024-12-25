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
#include "graphics/directX12/utils/dx_debug.hpp"

namespace reveal3d::graphics::dx12 {

class Texture {
public:
  struct InitInfo {
    window::Resolution res;
    DXGI_FORMAT format;
  };

  explicit Texture(InitInfo info, DescriptorHeap& heap) : handle_(heap.alloc()), buffer_(Buffer::buffer1d(1)) { }

  [[nodiscard]] auto& handle() { return srv_; }

  [[nodiscard]] auto resource() const { return buffer_.resource(); }

private:
  Buffer buffer_;
  DescriptorHandle srv_;
};

class RenderTexture {
public:
private:
  DescriptorHandle rtv_;
};

} // namespace reveal3d::graphics::dx12
