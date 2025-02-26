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
utils::ResourceArray<u32> deferredReleasesFlags;

void clean_deferred(u8 const index) {
  deferredReleasesFlags.at(index) = 0;

  if (!deferredReleases[index].empty()) {
    for (auto* resource: deferredReleases[Commands::frameIndex()]) {
      release(resource);
    }
    deferredReleases[index].clear();
  }
}


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
    clean_deferred(frame_index);
  }
}

void clean_all_resources() {
  for (auto [idx, flag]: std::views::enumerate(deferredReleasesFlags)) {
    if (flag) {
      clean_deferred(idx);
    }
  }
}

} // namespace reveal3d::graphics::dx12
