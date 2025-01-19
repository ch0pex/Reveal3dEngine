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

#include "dx_descriptor_heap_type.hpp"
#include "dx_descriptors.hpp"

namespace reveal3d::graphics::dx12 {

namespace detail {

template<HeapType Type>
struct ShaderVisibility {
  using value = std::true_type;
};

template<>
struct ShaderVisibility<HeapType::Dsv> {
  using value = std::false_type;
};

template<>
struct ShaderVisibility<HeapType::Rtv> {
  using value = std::false_type;
};

} // namespace detail

template<typename T>
concept resource = requires(T t, auto args) {
  // { T(std::declval<DescriptorHandle const&>(), args) };
  { t.resource() } -> std::same_as<ID3D12Resource*>;
  { t.handle() } -> std::same_as<DescriptorHandle const&>;
  { t.release() } -> std::same_as<void>;
};

template<HeapType Type>
concept is_shader_visible = std::same_as<typename detail::ShaderVisibility<Type>::value, std::true_type>;

}; // namespace reveal3d::graphics::dx12
