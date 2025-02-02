/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_resources.hpp
 * @version 1.0
 * @date 13/03/2024
 * @brief Short description

 * Policies:
 * - @struct Hard: Direct release, only when pipeline is blocked
 * - @struct Deferred: Deferred release of resources, mark and release the resources only when
 *   aren't being used by the gpu
 */

#pragma once

#include "dx_concepts.hpp"
#include "dx_deferring_system.hpp"
#include "dx_descriptor_heap_type.hpp"


namespace reveal3d::graphics::dx12::policy {

namespace detail {

template<typename Policy>
struct Base {
  static auto release(auto* resource) { Policy::release(resource); }
  static auto free_handle(auto& h, trivially_allocable_resource auto& resource) {
    Policy::free_handle(h, resource.handle());
  }
  static auto free_handle(auto& h, depth_shader_resource auto& resource) {
    Policy::free_handle(h.template heap<HeapType::Dsv>(), resource.dsv());
    Policy::free_handle(h.template heap<HeapType::Srv>(), resource.srv());
  }
  static auto free_handle(auto& h, render_shader_resource auto& resource) {
    Policy::free_handle(h.template heap<HeapType::Rtv>(), resource.dsv());
    Policy::free_handle(h.template heap<HeapType::Srv>(), resource.srv());
  }
};

struct HardImpl {
  static constexpr auto release     = [](auto* resource) { dx12::release(resource); };
  static constexpr auto free_handle = [](auto& heap, DescriptorHandle const& handle) { heap.free(handle); };
};

struct DeferredImpl {
  static constexpr auto release     = [](auto* resource) { deferred_release(resource); };
  static constexpr auto free_handle = [](auto& heap, DescriptorHandle const& handle) { heap.deferredFree(handle); };
};

} // namespace detail

using Hard     = detail::Base<detail::HardImpl>;
using Deferred = detail::Base<detail::DeferredImpl>;

} // namespace reveal3d::graphics::dx12::policy
