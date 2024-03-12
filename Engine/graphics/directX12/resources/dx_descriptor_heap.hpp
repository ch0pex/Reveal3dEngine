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


#include "../dx_common.hpp"

namespace reveal3d::graphics::dx::resources {

struct De


template <typename DescriptorType>
class DescriptorHeap {
public:
    explicit DescriptorHeap (const DescriptorHeap&) = delete;
    DescriptorHeap& operator=(const DescriptorHeap&) = delete;
    explicit DescriptorHeap(DescriptorHeap&&) = delete;
    DescriptorHeap& operator=(DescriptorHeap&&) = delete;

    bool Initialize(u32 capacity, bool isShaderVisible);
    void free();

private:
    ComPtr<ID3D12DescriptorHeap> heap_;
    CD3DX12_CPU_DESCRIPTOR_HANDLE  cpuStart_;
    CD3DX12_GPU_DESCRIPTOR_HANDLE  gpuStart_;
    u32 capacity_;
    u32 size_;
    u32 descriptorSize_;
    DescriptorType type_;

};

}
