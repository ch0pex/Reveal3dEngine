/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file concepts.hpp
 * @version 1.0
 * @date 25/12/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once


#include <concepts>
#include <d3d12.h>

struct DescriptorHandle;

namespace reveal3d::graphics::dx12 {

template<typename T>
concept GpuResource = requires(T t) {
  { t.resource() } -> std::same_as<ID3D12Resource*>;
  { t.handle() } -> std::same_as<DescriptorHandle&>;
};

}; // namespace reveal3d::graphics::dx12
