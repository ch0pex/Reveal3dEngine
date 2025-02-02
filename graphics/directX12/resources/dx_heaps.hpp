/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_heaps.hpp
 * @version 1.0
 * @date 31/01/2025
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "dx_descriptor_heap.hpp"

namespace reveal3d::graphics::dx12 {

class Heaps {
public:
  Heaps() : heaps_ {std::make_tuple(config::render.graphics.buffer_count, 1U, 1U)} { }

  ~Heaps() {
    logger(LogInfo) << "Releasing gpu heaps";
    cleanDeferreds();
  }

  template<HeapType Type>
  auto heap() -> decltype(auto) {
    return std::get<DescriptorHeap<Type>>(heaps_);
  }

  template<HeapType Type>
  auto heap() const -> decltype(auto) {
    return std::get<DescriptorHeap<Type>>(heaps_);
  }

  template<typename Type>
  auto heap() -> decltype(auto) {
    return std::get<Type>(heaps_);
  }

  template<typename Type>
  auto heap() const -> decltype(auto) {
    return std::get<Type>(heaps_);
  }

  template<resource T, typename... Args>
  T alloc(Args&&... args) = delete;

  template<trivially_allocable_resource Resource, typename... Args>
  Resource alloc(Args&&... args) {
    return {heap<typename Resource::heap_type>().alloc(), std::forward<Args>(args)...};
  }

  template<resource T>
  void free(T& resource) = delete;

  template<typename Policy = policy::Deferred, trivially_allocable_resource Resource>
  void free(Resource& r) {
    Policy::release(r.resource());
    Policy::free_handle(heap<typename Resource::heap_type>(), r);
  }

  void cleanDeferreds() {
    tuple::for_each(heaps_, [&](auto&& h) { h.cleanDeferreds(); });
  }

private:
  std::tuple<DescriptorHeap<HeapType::Rtv>, DescriptorHeap<HeapType::Dsv>, DescriptorHeap<HeapType::Srv>> heaps_;
};

} // namespace reveal3d::graphics::dx12
