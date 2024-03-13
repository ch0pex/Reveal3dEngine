/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_resources.cpp
 * @version 1.0
 * @date 12/03/2024
 * @brief Short description
 *
 * Longer description
 */

#include "dx_resources.hpp"
#include "../dx_commands.hpp"
#include "../dx_utils.hpp"

namespace reveal3d::graphics::dx {

std::vector<IUnknown*>          Resources::deferredReleases[frameBufferCount] {};
u32                             Resources::deferredReleasesFlags[frameBufferCount] {};

Resources::Resources() :
    rtvHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV),
    dsvHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV)
{
}


void Resources::SetDeferredFlag() {
    deferredReleasesFlags[Commands::FrameIndex()] = 1;
}

void Resources::CleanDeferreds() {
    // Will need mutex __declspec(noinline)
    deferredReleasesFlags[Commands::FrameIndex()] = 0;

    rtvHeap.CleanDeferreds();
    dsvHeap.CleanDeferreds();

    if (!deferredReleases[Commands::FrameIndex()].empty()) {
        for (auto* resource : deferredReleases[Commands::FrameIndex()]) {
            utl::release(resource);
        }
    }
}
}
