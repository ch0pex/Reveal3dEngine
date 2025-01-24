/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_resources.hpp
 * @version 1.0
 * @date 13/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include <common/primitive_types.hpp>

#include <d3d12.h>

namespace reveal3d::graphics::dx12 {

/**
 * @enum ReleasingPolicy
 * - Hard: Direct release, only when pipeline is blocked
 * - Deferred: Deferred release of resources, mark and release the resources only when
 *   aren't being used by the gpu
 */
enum class ReleasingPolicy : u8 {
  hard = 0,
  deferred,
};

template<typename T>
void release(T*& resource) {
  if (resource) {
    resource->Release();
    resource = nullptr;
  }
}

void deferred_release(IUnknown* resource);
void set_deferred_flag();
void clean_deferred_resources();
void clean_all_resources();

} // namespace reveal3d::graphics::dx12
