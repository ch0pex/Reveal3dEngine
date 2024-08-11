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
#include "dx_descriptor_heap.hpp"
#include "dx_upload_buffer.hpp"
#include "dx_deferring_system.hpp"
#include "render/material.hpp"

namespace reveal3d::graphics::dx12 {

struct FrameResource {
    ComPtr<ID3D12Resource> backBuffer;
    DescriptorHandle backBufferHandle;
    UploadBuffer<Constant<PerObjectData>> constantBuffer;
    UploadBuffer<Constant<GlobalShaderData>> passBuffer;
    UploadBuffer<Constant<render::Material>>  matBuffer;
};

}
