/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_resources.hpp
 * @version 1.0
 * @date 12/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "../dx_common.hpp"
#include "dx_descriptor_heap.hpp"

namespace reveal3d::graphics::dx {


struct Resources {

public:
    Resources();

    static void SetDeferredFlag();
    static void DeferredRelease();
    void CleanDeferreds();



    ComPtr<ID3D12Resource> renderTargets[frameBufferCount];
    //ComPtr<ID3D12Resource> depthStencilBuffer;

    /******************** Descriptor Heaps *************************/
    DescriptorHeap rtvHeap;
    DescriptorHeap dsvHeap;
    //DescriptorHeap srvHeap_;
    //DescriptorHeap uavHeap_;

    /******************  Deferred Release system ********************/
private:
    static std::vector<IUnknown*>          deferredReleases[frameBufferCount];
    static u32                             deferredReleasesFlags[frameBufferCount];
};

}
