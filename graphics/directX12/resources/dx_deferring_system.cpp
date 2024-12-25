/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_deferring_system.cpp
 * @version 1.0
 * @date 13/03/2024
 * @brief Short description
 *
 * Longer description
 */

#include "dx_deferring_system.hpp"
#include "graphics/directX12/dx_commands.hpp"
#include "graphics/directX12/utils/dx_resource_array.hpp"

#include <array>


namespace reveal3d::graphics::dx12 {

namespace {

std::array<std::vector<IUnknown*>, config::render.graphics.max_buffer_count> deferredReleases;
utl::ResourceArray<u32> deferredReleasesFlags;

} // namespace

void set_deferred_flag() { deferredReleasesFlags.at(Commands::frameIndex()) = 1; }

void deferred_release(IUnknown* resource) {
  if (resource != nullptr) {
    deferredReleases.at(Commands::frameIndex()).push_back(resource);
    deferredReleasesFlags.at(Commands::frameIndex()) = 1;
    resource                                         = nullptr;
  }
}

void clean_deferred_resources() {
  if (u8 const frame_index = Commands::frameIndex(); deferredReleasesFlags.at(frame_index)) {

    deferredReleasesFlags.at(frame_index) = 0;

    if (!deferredReleases[frame_index].empty()) {
      for (auto* resource: deferredReleases[Commands::frameIndex()]) {
        release(resource);
      }
      deferredReleases[frame_index].clear();
    }
  }
}

void clean_all_resources() {
  for (auto [idx, flag]: std::views::enumerate(deferredReleasesFlags)) {
    if (flag) {

      flag = 0;

      if (!deferredReleases[idx].empty()) {
        for (auto* resource: deferredReleases[Commands::frameIndex()]) {
          release(resource);
        }
        deferredReleases[idx].clear();
      }
    }
  }
}

} // namespace reveal3d::graphics::dx12
