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

struct DescriptorHandle {
    CD3DX12_CPU_DESCRIPTOR_HANDLE  cpuPos;
    CD3DX12_GPU_DESCRIPTOR_HANDLE  gpuPos;

    inline bool IsValid() { return cpuPos.ptr != 0; }
    inline bool IsShaderVisible() { return gpuPos.ptr != 0; }

#ifdef _DEBUG // For debugging porpouse save Descritpor heap pointer
    friend class DescriptorHeap;
    DescriptorHeap* container { nullptr };
    u32 index {0};
#endif
};

//TODO: Concept for checking Descriptor type is D3D12_DESCRIPTOR_HEAP_TYPE
template <typename DescriptorType>
class DescriptorHeap {
public:
    explicit DescriptorHeap (const DescriptorHeap&) = delete;
    DescriptorHeap& operator=(const DescriptorHeap&) = delete;
    explicit DescriptorHeap(DescriptorHeap&&) = delete;
    DescriptorHeap& operator=(DescriptorHeap&&) = delete;

    INLINE D3D12_DESCRIPTOR_HEAP_TYPE Type() { return type_; }
    INLINE D3D12_CPU_DESCRIPTOR_HANDLE CpuStart() { return cpuStart_; }
    INLINE D3D12_GPU_DESCRIPTOR_HANDLE GpuStart() { return gpuStart_; }
    INLINE u32 Capacity() { return capacity_; }
    INLINE u32 Size() { return size_; }
    INLINE u32 DescriptorSize() { return capacity_; }
    INLINE bool IsShaderVisible() { return gpuStart_.ptr != 0; }


    bool Initialize(u32 capacity, bool isShaderVisible);
    [[nodiscard]] DescriptorHandle alloc();
    void free(DescriptorHandle& handle);

private:
    ComPtr<ID3D12DescriptorHeap> heap_;
    CD3DX12_CPU_DESCRIPTOR_HANDLE cpuStart_ {};
    CD3DX12_GPU_DESCRIPTOR_HANDLE gpuStart_ {};
    std::unique_ptr<u32[]> freeHandles_ {};
    u32 capacity_ { 0 };
    u32 size_ { 0 };
    u32 descriptorSize_ {};
    DescriptorType type_ {};

};

}
