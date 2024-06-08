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
#include "dx_deferring_system.hpp"
#include "render/vertex.hpp"

namespace reveal3d::graphics::dx {

struct BufferInitInfo {
    ID3D12Device *device;
    ID3D12GraphicsCommandList *cmdList;
    const void *data{};
    u64 count {0};
    DXGI_FORMAT format{};
};

template<typename BUFFER_VIEW_TYPE, typename T>
class Buffer {
public:
    Buffer() = default;
    explicit Buffer(BufferInitInfo &info) { Init(info); }
    void Init(BufferInitInfo &info);
    void Release() { DeferredRelease(buff_); }

    /* NOTE: we will need a custom vector in a future
    explicit Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    explicit Buffer(Buffer&&) = delete;
    Buffer& operator=(Buffer&&) = delete;
    */

    [[nodiscard]]INLINE ID3D12Resource* Get() const { return buff_; };
    [[nodiscard]]INLINE BUFFER_VIEW_TYPE* View() { return &view_; };
    [[nodiscard]]INLINE u32 Count() const { return count_; };
    [[nodiscard]]INLINE u32 SizeInBytes() const { return size_; };

private:
    void SetView(BufferInitInfo &info);
    ID3D12Resource *buff_;
    BUFFER_VIEW_TYPE view_;
    u32 size_ { 0 };
    u32 count_ { 0 };
};

using VertexBuffer = Buffer<D3D12_VERTEX_BUFFER_VIEW, render::Vertex>;
using IndexBuffer = Buffer<D3D12_INDEX_BUFFER_VIEW, u32>;

template<typename BUFFER_VIEW_TYPE, typename T>
void Buffer<BUFFER_VIEW_TYPE, T>::Init(BufferInitInfo &info)
{
    count_ = info.count;
    size_ = sizeof(T) * info.count;
    auto uploadBuffer = UploadBuffer<T>(); //Upload buffer is created in order to store buffer in gpu
    auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    auto resDesc = CD3DX12_RESOURCE_DESC::Buffer(size_);

    uploadBuffer.Init(info.device, info.count);

    info.device->CreateCommittedResource(
            &heapProperties,
            D3D12_HEAP_FLAG_NONE,
            &resDesc,
            D3D12_RESOURCE_STATE_COMMON,
            nullptr,
            IID_PPV_ARGS(&buff_)) >> utl::DxCheck;


    D3D12_SUBRESOURCE_DATA subResourceData = {};
    subResourceData.pData = info.data;
    subResourceData.RowPitch = size_;
    subResourceData.SlicePitch = subResourceData.RowPitch;

    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(buff_,
                                                        D3D12_RESOURCE_STATE_COMMON,
                                                        D3D12_RESOURCE_STATE_COPY_DEST);
    info.cmdList->ResourceBarrier(1, &barrier);
    uploadBuffer.CopyData(0, (T *) info.data, info.count); //Copying cpu info to upload buvffer
    info.cmdList->CopyResource(buff_, uploadBuffer.Get()); // Coping upload buffer into buffer

    barrier = CD3DX12_RESOURCE_BARRIER::Transition(buff_,
                                                   D3D12_RESOURCE_STATE_COPY_DEST,
                                                   D3D12_RESOURCE_STATE_GENERIC_READ);
    info.cmdList->ResourceBarrier(1, &barrier);

    uploadBuffer.Release();
    SetView(info);
}

template<>
inline void Buffer<D3D12_VERTEX_BUFFER_VIEW, render::Vertex>::SetView(BufferInitInfo &info) {
    view_.BufferLocation = buff_->GetGPUVirtualAddress();
    view_.SizeInBytes = size_;
    view_.StrideInBytes = sizeof(render::Vertex);
}

template<>
inline void Buffer<D3D12_INDEX_BUFFER_VIEW, u32>::SetView(BufferInitInfo &info) {
    view_.BufferLocation = buff_->GetGPUVirtualAddress();
    view_.SizeInBytes = size_;
    view_.Format= info.format;
}

}