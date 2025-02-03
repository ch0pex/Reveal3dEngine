/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_render_texture.hpp
 * @version 1.0
 * @date 29/01/2025
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "dx_heaps.hpp"
#include "dx_texture.hpp"

namespace reveal3d::graphics::dx12 {


class RenderTexture {
public:
  using init_info = Texture::init_info;

  RenderTexture(Texture&& texture, std::vector<DescriptorHandle>&& rtv_descriptors) :
    texture_(std::move(texture)), rtv_(std::move(rtv_descriptors)) { }

  RenderTexture(RenderTexture&& other) noexcept : texture_(std::move(other.texture_)), rtv_(std::move(other.rtv_)) { }

  [[nodiscard]] auto resource() const { return texture_.resource(); }

  auto& rtv(u32 const index = 0) const { return rtv_.at(index); }

  auto& srv() const { return texture_.handle(); }

private:
  Texture texture_;
  std::vector<DescriptorHandle> rtv_ {};
};

/**
 * RenderTextures need a specialization of alloc heap member method
 * @param info Texture creation information
 * @return RenderTexture
 */
template<>
inline RenderTexture Heaps::alloc<RenderTexture>(Texture::init_info&& info) {
  assert(info.res_desc.MipLevels <= Texture::max_mips);

  Texture texture {heap<HeapType::Srv>().alloc(), info};
  std::vector<DescriptorHandle> rtv_descriptors;

  rtv_descriptors.reserve(info.res_desc.MipLevels);

  D3D12_RENDER_TARGET_VIEW_DESC desc {
    .Format = info.res_desc.Format, .ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D, .Texture2D = {.MipSlice = 0}
  };

  for (u32 i {0}; i < info.res_desc.MipLevels; ++i) {
    rtv_descriptors.emplace_back(heap<HeapType::Rtv>().alloc());
    adapter.device->CreateRenderTargetView(texture.resource(), &desc, rtv_descriptors.at(i).cpu);
    ++desc.Texture2D.MipSlice;
  }

  return {std::move(texture), std::move(rtv_descriptors)};
}

} // namespace reveal3d::graphics::dx12
