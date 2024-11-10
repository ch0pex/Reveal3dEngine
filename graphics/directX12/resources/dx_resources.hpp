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
  DescriptorHandle depth_buffer_handle;
  UploadBuffer<Constant<PerObjectData>> constant_buffer;
  UploadBuffer<Constant<GlobalShaderData>> pass_buffer;
  UploadBuffer<Constant<render::Material>> mat_buffer;
};

class FrameResources {
public:
  FrameResources() = default;

  void init(ID3D12Device* device) {
    for (auto& frame_resource: frame_resources_) {
      frame_resource.constant_buffer.init(device, 100'000);
      frame_resource.mat_buffer.init(device, 100'000);
      frame_resource.pass_buffer.init(device, 1U);
    }
  }


private:
  utl::ResourceArray<FrameResource> frame_resources_;
};

} // namespace reveal3d::graphics::dx12
