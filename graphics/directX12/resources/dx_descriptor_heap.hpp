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

#include "dx_deferring_system.hpp"

#include <array>

namespace reveal3d::graphics::dx12 {

struct DescriptorHandle {
    D3D12_CPU_DESCRIPTOR_HANDLE  cpu {};
    D3D12_GPU_DESCRIPTOR_HANDLE  gpu {};
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

    //inline D3D12_DESCRIPTOR_HEAP_TYPE GetType() { return static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(Type); }
    inline ID3D12DescriptorHeap* get() { return heap_; };
    inline D3D12_CPU_DESCRIPTOR_HANDLE cpuStart() { return cpu_start_; }
    inline D3D12_GPU_DESCRIPTOR_HANDLE gpuStart() { return gpu_start_; }
    inline u32 capacity() { return capacity_; }
    inline u32 size() { return size_; }
    inline u32 descriptorSize() { return capacity_; }
    inline bool isShaderVisible() { return gpu_start_.ptr != 0; }

    bool initialize(ID3D12Device *constdevice, u32 capacity, bool is_shader_visible);
    [[nodiscard]] DescriptorHandle alloc();
    void free(DescriptorHandle& handle);
    void cleanDeferreds();
    void release();

private:
    ID3D12DescriptorHeap *heap_ { nullptr };
    D3D12_CPU_DESCRIPTOR_HANDLE cpu_start_{};
    D3D12_GPU_DESCRIPTOR_HANDLE gpu_start_{};
    std::unique_ptr<u32[]> free_handles_{};
    std::array<std::vector<u32>, frameBufferCount> deferred_indices_;
    u32 capacity_ { 0 };
    u32 size_ { 0 };
    u32 descriptor_size_{};
    D3D12_DESCRIPTOR_HEAP_TYPE type_;

    //std::mutexmutex in future
};


struct Heaps {

public:
    Heaps();
    void release();

    /******************** Descriptor Heaps *************************/
    DescriptorHeap rtv;
    DescriptorHeap dsv;
//    DescriptorHeap cbv;
    DescriptorHeap srv; //TODO
//    DescriptorHeap uavHeap; //TODO
};

}
