/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_descriptor_heap.hpp
 * @version 1.0
 * @date 12/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "dx_resources.hpp"

namespace reveal3d::graphics::dx {

struct DescriptorHandle {
    CD3DX12_CPU_DESCRIPTOR_HANDLE  cpu {};
    CD3DX12_GPU_DESCRIPTOR_HANDLE  gpu {};
    u32 index {};


    inline bool IsValid() { return cpu.ptr != 0; }
    inline bool IsShaderVisible() { return gpu.ptr != 0; }
/*
#ifdef _DEBUG // For debugging porpouse save Descritpor heap pointer
    friend class DescriptorHeap;
    DescriptorHeap* container { nullptr };
    u32 index {0};
#endif
*/
};


//TODO: Concept for checking Descriptor type is D3D12_DESCRIPTOR_HEAP_TYPE
class DescriptorHeap {
public:
    explicit DescriptorHeap (D3D12_DESCRIPTOR_HEAP_TYPE type) : type_(type) {}
    explicit DescriptorHeap (const DescriptorHeap&) = delete;
    DescriptorHeap& operator=(const DescriptorHeap&) = delete;
    explicit DescriptorHeap(DescriptorHeap&&) = delete;
    DescriptorHeap& operator=(DescriptorHeap&&) = delete;
    ~DescriptorHeap() = default;

    //INLINE D3D12_DESCRIPTOR_HEAP_TYPE GetType() { return static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(Type); }
    INLINE D3D12_CPU_DESCRIPTOR_HANDLE CpuStart() { return cpuStart_; }
    INLINE D3D12_GPU_DESCRIPTOR_HANDLE GpuStart() { return gpuStart_; }
    INLINE u32 Capacity() { return capacity_; }
    INLINE u32 Size() { return size_; }
    INLINE u32 DescriptorSize() { return capacity_; }
    INLINE bool IsShaderVisible() { return gpuStart_.ptr != 0; }

    bool Initialize(ID3D12Device *device, u32 capacity, bool isShaderVisible);
    [[nodiscard]] DescriptorHandle alloc();
    void free(DescriptorHandle& handle);
    void CleanDeferreds();
    void Release();

private:
    ID3D12DescriptorHeap *heap_ { nullptr };
    D3D12_CPU_DESCRIPTOR_HANDLE cpuStart_ {};
    D3D12_GPU_DESCRIPTOR_HANDLE gpuStart_ {};
    std::unique_ptr<u32[]> freeHandles_ {};
    std::vector<u32> deferredIndices_[frameBufferCount];
    u32 capacity_ { 0 };
    u32 size_ { 0 };
    u32 descriptorSize_ {};
    D3D12_DESCRIPTOR_HEAP_TYPE type_;

    //std::mutexmutex in future
};


struct Heaps {

public:
    Heaps();
    ~Heaps();

    /******************** Descriptor Heaps *************************/
    DescriptorHeap rtvHeap;
    DescriptorHeap dsvHeap;
//    DescriptorHeap srvHeap; //TODO
//    DescriptorHeap uavHeap; //TODO
};

}
