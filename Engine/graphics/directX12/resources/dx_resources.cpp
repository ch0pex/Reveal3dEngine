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

#include "dx_resources.hpp"
#include "../dx_commands.hpp"



namespace reveal3d::graphics::dx {

std::vector<IUnknown*> deferredReleases[frameBufferCount];
u32 deferredReleasesFlags[frameBufferCount];

void SetDeferredFlag() {
    deferredReleasesFlags[Commands::FrameIndex()] = 1;
}

void DeferredRelease(IUnknown* resource) {
    if (resource != nullptr) {
        deferredReleases[Commands::FrameIndex()].push_back(resource);
        deferredReleasesFlags[Commands::FrameIndex()] = 1;
        resource = nullptr;
    }
}

void CleanDeferredResources(Heaps& heaps) {
    // Will need mutex __declspec(noinline)
    const u8 frameIndex = Commands::FrameIndex();

    if (deferredReleasesFlags[frameIndex]) {

        deferredReleasesFlags[frameIndex] = 0;

        heaps.rtv.CleanDeferreds();
        heaps.dsv.CleanDeferreds();
        //heaps.cbv.CleanDeferreds();
        // uavHeap.CleanDeferreds();
        // uavHeap.CleanDeferreds();

        if (!deferredReleases[frameIndex].empty()) {
            for (auto* resource : deferredReleases[Commands::FrameIndex()]) {
                utl::release(resource);
            }
            deferredReleases[frameIndex].clear();
        }
    }
}

}