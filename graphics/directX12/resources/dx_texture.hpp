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

  Texture(DescriptorHandle const& handle, init_info const& info) : buffer_(info), srv_(handle) {

    D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
    srv_desc.Shader4ComponentMapping         = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srv_desc.Format                          = info.res_desc.Format;
    srv_desc.ViewDimension                   = D3D12_SRV_DIMENSION_TEXTURE2D;
    srv_desc.Texture2D.MostDetailedMip       = 0;
    srv_desc.Texture2D.MipLevels             = 1;

    adapter.device->CreateShaderResourceView(buffer_.resource(), &srv_desc, srv_.cpu);
  }

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
