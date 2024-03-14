/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_buffer.hpp
 * @version 1.0
 * @date 13/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "../dx_common.hpp"

namespace reveal3d::graphics::dx {

struct BufferInfo {
    ID3D12Device *device;
    ID3D12GraphicsCommandList *cmdList;
    void *data{};
    u64 byteSize {0};
    u32 byteStride {0};

};


template<typename BUFFER_VIEW_TYPE>
class Buffer {
public:
    Buffer() = default;
    void Upload(BufferInfo &info);
    ~Buffer() { DeferredRelease(buff_); }

    explicit Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    explicit Buffer(Buffer&&) = delete;
    Buffer& operator=(Buffer&&) = delete;

    [[nodiscard]]inline ID3D12Resource* Get() const { return buff_; };
    [[nodiscard]]inline BUFFER_VIEW_TYPE* View() const { return view_; };
    [[nodiscard]]inline u32 Size() const { return size_; };

private:
    ID3D12Resource *buff_;
    BUFFER_VIEW_TYPE view_;
    u32 size_ { 0 };
};

template<typename BUFFER_VIEW_TYPE>
void Buffer<BUFFER_VIEW_TYPE>::Upload(BufferInfo &info)
{
    ID3D12Resource *uploadBuffer = nullptr;
    auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    auto resDesc = CD3DX12_RESOURCE_DESC::Buffer(info.byteSize);

    info.device->CreateCommittedResource(
            &heapProperties,
            D3D12_HEAP_FLAG_NONE,
            &resDesc,
            D3D12_RESOURCE_STATE_COMMON,
            nullptr,
            IID_PPV_ARGS(&buff_)) >> utl::DxCheck;

    heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    info.device->CreateCommittedResource(
            &heapProperties,
            D3D12_HEAP_FLAG_NONE,
            &resDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&uploadBuffer)) >> utl::DxCheck;

    D3D12_SUBRESOURCE_DATA subResourceData = {};
    subResourceData.pData = info.data;
    subResourceData.RowPitch = info.byteSize;
    subResourceData.SlicePitch = subResourceData.RowPitch;

    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(buff_,
                                          D3D12_RESOURCE_STATE_COMMON,
                                          D3D12_RESOURCE_STATE_COPY_DEST);
    info.cmdList->ResourceBarrier(1, &barrier);
    uploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&info.data)) >> utl::DxCheck;

    barrier = CD3DX12_RESOURCE_BARRIER::Transition(buff_,
                                          D3D12_RESOURCE_STATE_COPY_DEST,
                                          D3D12_RESOURCE_STATE_GENERIC_READ);
    info.cmdList->ResourceBarrier(1, &barrier);

    DeferredRelease(uploadBuffer);
    view_.BufferLocation = buff_->GetGPUVirtualAddress();
    view_.StrideInBytes = info.byteStride;
    view_.SizeInBytes = size_;
}

}
