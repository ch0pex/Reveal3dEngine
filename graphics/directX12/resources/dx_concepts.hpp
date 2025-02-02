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

template<HeapType Type>
class DescriptorHeap;

template<typename T>
concept resource = requires(T t, auto args) {
  typename T::init_info;
  { t.resource() } -> std::same_as<ID3D12Resource*>;
};

template<typename T>
concept trivially_allocable_resource = resource<T> and requires(T t) {
  typename T::heap_type;
  { t.handle() } -> std::same_as<DescriptorHandle const&>;
};

template<typename T, HeapType Type>
concept resource_type = resource<T> and requires(T t, auto args) {
  { std::same_as<typename T::heap_type, DescriptorHeap<Type>> };
};

template<typename T>
concept sampler_resource = resource_type<T, HeapType::Sampler>;

template<typename T>
concept unordered_access_resource = resource_type<T, HeapType::Uav>;

template<typename T>
concept shader_resource = resource_type<T, HeapType::Srv>;

template<typename T>
concept constant_resource = resource_type<T, HeapType::Cbv>;

template<typename T>
concept render_target_resource = resource_type<T, HeapType::Rtv>;

template<typename T>
concept depth_stencil_resource = resource_type<T, HeapType::Dsv>;

template<typename T>
concept depth_shader_resource = requires(T resource) {
  { resource.srv() } -> std::same_as<DescriptorHandle const&>;
  { resource.dsv() } -> std::same_as<DescriptorHandle const&>;
};

template<typename T>
concept render_shader_resource = requires(T resource) {
  { resource.rtv() } -> std::same_as<DescriptorHandle const&>;
  { resource.srv() } -> std::same_as<DescriptorHandle const&>;
};

template<HeapType Type>
concept is_shader_visible = std::same_as<typename detail::ShaderVisibility<Type>::value, std::true_type>;

constexpr bool shader_visible(HeapType const type) { return type != HeapType::Dsv && type != HeapType::Rtv; }

}; // namespace reveal3d::graphics::dx12
