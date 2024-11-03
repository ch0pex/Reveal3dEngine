/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_resources.cpp
 * @version 1.0
 * @date 13/03/2024
 * @brief Short description
 *
 * Longer description
 */

#include "dx_deferring_system.hpp"
#include "../dx_commands.hpp"
#include "dx_descriptor_heap.hpp"

#include <array>


namespace reveal3d::graphics::dx12 {

std::array<std::vector<IUnknown*>, config::graphics.buffer_count> deferredReleases;
std::array<u32, config::graphics.buffer_count> deferredReleasesFlags;

void set_deferred_flag() { deferredReleasesFlags[Commands::frameIndex()] = 1; }

void deferred_release(IUnknown* resource) {
  if (resource != nullptr) {
    deferredReleases[Commands::frameIndex()].push_back(resource);
    deferredReleasesFlags[Commands::frameIndex()] = 1;
    resource                                      = nullptr;
  }
}

void clean_deferred_resources(Heaps& heaps) {
  // Will need mutex __declspec(noinline)

  if (const u8 frame_index = Commands::frameIndex(); deferredReleasesFlags[frame_index]) {

    deferredReleasesFlags[frame_index] = 0;

    heaps.rtv.cleanDeferreds();
    heaps.dsv.cleanDeferreds();
    // heaps.cbv.cleanDeferreds();
    //  uavHeap.cleanDeferreds();
    //  uavHeap.cleanDeferreds();

    if (!deferredReleases[frame_index].empty()) {
      for (auto* resource: deferredReleases[Commands::frameIndex()]) {
        utl::release(resource);
      }
      deferredReleases[frame_index].clear();
    }
  }
}

} // namespace reveal3d::graphics::dx12
