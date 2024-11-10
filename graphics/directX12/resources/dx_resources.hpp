/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_resources.hpp
 * @version 1.0
 * @date 25/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "dx_buffer.hpp"
#include "dx_deferring_system.hpp"
#include "dx_descriptor_heap.hpp"
#include "dx_upload_buffer.hpp"
#include "graphics/constants.hpp"
#include "graphics/directX12/dx_surface.hpp"
#include "render/material.hpp"

namespace reveal3d::graphics::dx12 {

struct FrameResource {
  ComPtr<ID3D12Resource> back_buffer;
  DescriptorHandle back_buffer_handle;
  DescriptorHandle depth_buffer_handle;
  UploadBuffer<Constant<PerObjectData>> constant_buffer;
  UploadBuffer<Constant<GlobalShaderData>> pass_buffer;
  UploadBuffer<Constant<render::Material>> mat_buffer;
};

class FrameResources {
public:
  using container      = std::array<FrameResource, config::Render::Graphics::max_buffer_count>;
  using iterator       = container::iterator;
  using const_iterator = container::const_iterator;

  FrameResources() = default;

  void initRTVs(ID3D12Device* device, Surface const& surface, Heaps* heaps = nullptr) {
    constexpr D3D12_RENDER_TARGET_VIEW_DESC desc = {
      .Format = DXGI_FORMAT_R8G8B8A8_UNORM, .ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D
    };
    for (auto [idx, frame_resource]: std::views::enumerate(frame_resources_)) {
      if (heaps != nullptr) {
        frame_resource.back_buffer_handle = heaps->rtv.alloc();
      }
      surface.getBuffer(idx, frame_resource.back_buffer);
      device->CreateRenderTargetView(frame_resource.back_buffer.Get(), &desc, frame_resource.back_buffer_handle.cpu);
      std::wstring name = L"BackBuffer " + std::to_wstring(idx);
      frame_resource.back_buffer->SetName(name.c_str()) >> utl::DxCheck;
    }
  }

  void initCBs(ID3D12Device* device) {
    for (auto& frame_resource: frame_resources_) {
      frame_resource.constant_buffer.init(device, 100'000);
      frame_resource.mat_buffer.init(device, 100'000);
      frame_resource.pass_buffer.init(device, 1U);
    }
  }

  [[nodiscard]] const_iterator begin() const { return const_iterator(frame_resources_.begin()); }

  [[nodiscard]] const_iterator end() const {
    return const_iterator(frame_resources_.begin() + config::render.graphics.buffer_count);
  }

  iterator begin() { return iterator(frame_resources_.begin()); }

  iterator end() { return iterator(frame_resources_.begin() + config::render.graphics.buffer_count); }

  FrameResource& at(u8 const index) {
    assert(index < config::render.graphics.buffer_count);
    return frame_resources_.at(index);
  }

private:
  container frame_resources_;
};

} // namespace reveal3d::graphics::dx12
