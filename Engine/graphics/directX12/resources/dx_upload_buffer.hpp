/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_upload_buffer.hpp
 * @version 1.0
 * @date 19/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include  "../dx_common.hpp"
#include "math/math.hpp"
#include "dx_descriptor_heap.hpp"

namespace reveal3d::graphics::dx {

struct PassConstant {
    math::mat4 view;
    math::mat4 invView;
    math::mat4 proj;
    math::mat4 invProj;
    math::mat4 viewProj;
    math::mat4 invViewProj;
    math::vec2 cbPerObjectPad1;
    math::vec2 renderTargetSize;
    f32 nearZ;
    f32 farZ;
    f32 totalTime;
    f32 deltaTime;
};

struct ObjConstant {
    ObjConstant() : world(XMMatrixIdentity()) {}
    math::mat4 world;
};

// S denotes the necesary amount of 256 bytes for alignment
template<typename T, u32 S>
union AlignedConstant {
    AlignedConstant() : data() {}
    T data;
    uint8_t alignmentPadding[D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT][S];
};

//TODO: Improve Upload buffer and buffer to dynamic like heaps
template<typename T>
class UploadBuffer {
public:
    UploadBuffer() = default;
    explicit UploadBuffer(const UploadBuffer&) = delete;
    UploadBuffer& operator=(const UploadBuffer&) = delete;
    explicit UploadBuffer(UploadBuffer&&) = delete;
    UploadBuffer& operator=(UploadBuffer&&) = delete;

    void Init(ID3D12Device *device, u32 count);
    DescriptorHandle CreateView(ID3D12Device *device, DescriptorHeap &heap);
    INLINE ID3D12Resource *Get() { return buff_; };
    INLINE u32 Size() { return capacity_; };
    INLINE D3D12_GPU_VIRTUAL_ADDRESS GpuStart() { return buff_->GetGPUVirtualAddress(); }
    INLINE D3D12_GPU_VIRTUAL_ADDRESS GpuPos(u32 index) { return buff_->GetGPUVirtualAddress() + (index * sizeof(T)); }

    //bool Add(const T* data, u32 count); TODO
    void CopyData(u32 elementIndex, const T* data, u32 count = 1) { memcpy(&mappedData_[elementIndex], data, sizeof(T) * count); }
    void Release() { if (buff_ != nullptr) buff_->Unmap(0, nullptr); DeferredRelease(buff_); }
    T* mappedData_ { nullptr };
private:
    ID3D12Resource *buff_;
    u32 capacity_ { 0 };
    u32 size_ { 0 };
};


template<typename T>
void UploadBuffer<T>::Init(ID3D12Device *device, u32 count) {
    capacity_ = sizeof(T) * count;
    auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    auto resDesc = CD3DX12_RESOURCE_DESC::Buffer(capacity_);

    device->CreateCommittedResource(
            &heapProperties,
            D3D12_HEAP_FLAG_NONE,
            &resDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&buff_)) >> utl::DxCheck;

    // TODO: add option to hide this to cpu with range(0,0)
    buff_->Map(0, nullptr, reinterpret_cast<void**>(&mappedData_));


}

template<typename T>
DescriptorHandle UploadBuffer<T>::CreateView(ID3D12Device *device, DescriptorHeap &heap) {
    const u64 buffAddress = GpuStart() + (sizeof(T) * size_++);
    const D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {
            .BufferLocation = buffAddress,
            .SizeInBytes = sizeof(T)
    };

    DescriptorHandle handle = heap.alloc();
    device->CreateConstantBufferView(&desc, handle.cpu);
    return handle;
}


using ConstantBuffer = UploadBuffer<AlignedConstant<ObjConstant, 1>>;
using PassCB = UploadBuffer<AlignedConstant<PassConstant, 2>>;

}
