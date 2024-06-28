/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_descriptor_heap.cpp
 * @version 1.0
 * @date 12/03/2024
 * @brief Short description
 *
 * Longer description
 */

#include "dx_descriptor_heap.hpp"
#include "../dx_commands.hpp"

namespace reveal3d::graphics::dx12 {

bool DescriptorHeap::Initialize(ID3D12Device * const device, u32 capacity, bool isShaderVisible) {

    assert(capacity && capacity < D3D12_MAX_SHADER_VISIBLE_DESCRIPTOR_HEAP_SIZE_TIER_2);
    assert(!(type_ == D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER &&
             capacity > D3D12_MAX_SHADER_VISIBLE_SAMPLER_HEAP_SIZE));

    if (type_ == D3D12_DESCRIPTOR_HEAP_TYPE_DSV ||
        type_ == D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
    {
        isShaderVisible = false;
    }

    Release();

    D3D12_DESCRIPTOR_HEAP_DESC desc{
            .Type = type_,
            .NumDescriptors = capacity,
            .Flags = isShaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
            .NodeMask = 0,
    };

    if(FAILED(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap_)))){
        Release();
        return false;
    }

    freeHandles_ = std::move(std::make_unique<u32[]>(capacity));
    capacity_ = capacity;
    size_ = 0;

    for (u32 i = 0 ; i < capacity; ++i) {
        freeHandles_[i] = i;
    }

    //DEBUG_ACTION(for (u32 i{ 0 }; i < frameBufferCount; ++i) assert(_deferred_free_indices[i].empty()));
    descriptorSize_ = device->GetDescriptorHandleIncrementSize(type_);
    cpuStart_ = heap_->GetCPUDescriptorHandleForHeapStart();
    gpuStart_ = isShaderVisible ? heap_->GetGPUDescriptorHandleForHeapStart() : D3D12_GPU_DESCRIPTOR_HANDLE{ 0 };

    return true;
}

DescriptorHandle DescriptorHeap::alloc() {
    assert(heap_);
    assert(size_ < capacity_);

    const u32 index{ freeHandles_[size_] };
    const u32 offset{ index * descriptorSize_  };
    ++size_;

    DescriptorHandle handle;
    handle.cpu.ptr = cpuStart_.ptr + offset;
    if (IsShaderVisible()) {
        handle.gpu.ptr = gpuStart_.ptr + offset;
    }

    handle.index = index;
    //DEBUG_OP(backBufferHandle.container = this);
    return handle;
}

void DescriptorHeap::free(DescriptorHandle &handle) {
    if (!handle.IsValid()) return;
    assert(heap_ && size_);
    assert(handle.cpu.ptr >= cpuStart_.ptr);
    assert((handle.cpu.ptr - cpuStart_.ptr) % descriptorSize_ == 0);
    assert(handle.index < capacity_);
    const u32 index{ (u32)(handle.cpu.ptr - cpuStart_.ptr) / descriptorSize_ };
    assert(handle.index == index);

    const u32 frame_idx = Commands::FrameIndex();
    deferredIndices_[frame_idx].push_back(index);
    SetDeferredFlag();
    handle = {};
}

void DescriptorHeap::CleanDeferreds() {
    std::vector<u32>& indices{ deferredIndices_[Commands::FrameIndex()] };
    if (!indices.empty())
    {
        for (auto index : indices) {
            --size_;
            freeHandles_[size_] = index; // For cleaning Deferreds we just set them in free handles
        }
        indices.clear();
    }
}

void DescriptorHeap::Release() {
    DeferredRelease(heap_);
}



Heaps::Heaps() :
    rtv(D3D12_DESCRIPTOR_HEAP_TYPE_RTV),
    dsv(D3D12_DESCRIPTOR_HEAP_TYPE_DSV),
    srv(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
//    cbv(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
//        uavHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
{
}
void Heaps::Release() {
    rtv.Release();
    dsv.Release();
    srv.Release();
//    cbv.Release();
    //    uavHeap.Release();
}

}
