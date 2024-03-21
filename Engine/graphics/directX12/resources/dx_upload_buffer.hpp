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

namespace reveal3d::graphics::dx {

struct ObjConstant {
    ObjConstant(f32 scalar ) : offset(scalar) {}
    math::xvec4 offset;
};

union AlignedObjCosntant {
    AlignedObjCosntant() : constant(0.0f) {}
    ObjConstant constant;
    uint8_t alignmentPadding[D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT];
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
    inline ID3D12Resource *Get() { return buff_; };
    inline u32 Size() { return capacity_; };
    inline D3D12_GPU_VIRTUAL_ADDRESS GpuAddress() { return buff_->GetGPUVirtualAddress(); }


    //bool Add(const T* data, u32 count); TODO
    void CopyData(u32 elementIndex, const T* data, u32 count = 1) { memcpy(&mappedData_[elementIndex], data, sizeof(T) * count); }
    void Release() { if (buff_ != nullptr) buff_->Unmap(0, nullptr); DeferredRelease(buff_); }
private:
    ID3D12Resource *buff_;
    T* mappedData_ { nullptr };
    u32 capacity_{ 0 };
    //T* offset { nullptr };
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

using ConstantBuffer = UploadBuffer<AlignedObjCosntant>;

}
