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

namespace reveal3d::graphics::dx12 {

struct BufferInitInfo {
    ID3D12Device *device;
    ID3D12GraphicsCommandList *cmd_list;
    const void *data{};
    u64 count {0};
    DXGI_FORMAT format{};
};

template<typename BUFFER_VIEW_TYPE, typename T>
class Buffer {
public:
    Buffer() = default;
    explicit Buffer(BufferInitInfo &info) { init(info); }
    void init(BufferInitInfo &info);
    void release() { deferred_release(buff_); }

    /* NOTE: we will need a custom vector in a future
    explicit Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    explicit Buffer(Buffer&&) = delete;
    Buffer& operator=(Buffer&&) = delete;
    */

    [[nodiscard]]inline ID3D12Resource* Get() const { return buff_; };
    [[nodiscard]]inline BUFFER_VIEW_TYPE*view() { return &view_; };
    [[nodiscard]]inline u32 count() const { return count_; };
    [[nodiscard]]inline u32 SizeInBytes() const { return size_; };

private:
    void setView(BufferInitInfo &info);
    ID3D12Resource *buff_;
    BUFFER_VIEW_TYPE view_;
    u32 size_ { 0 };
    u32 count_ { 0 };
};

using VertexBuffer = Buffer<D3D12_VERTEX_BUFFER_VIEW, render::Vertex>;
using IndexBuffer = Buffer<D3D12_INDEX_BUFFER_VIEW, u32>;

static u32 mycounter = 0;

template<typename BUFFER_VIEW_TYPE, typename T>
void Buffer<BUFFER_VIEW_TYPE, T>::init(BufferInitInfo &info)
{
    count_ = info.count;
    size_ = sizeof(T) * info.count;
    auto upload_buffer = UploadBuffer<T>(); //Upload buffer is created in order to store buffer in gpu
    auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    auto res_desc = CD3DX12_RESOURCE_DESC::Buffer(size_);

    upload_buffer.init(info.device, info.count);

    info.device->CreateCommittedResource(
            &heapProperties,
            D3D12_HEAP_FLAG_NONE,
            &res_desc,
            D3D12_RESOURCE_STATE_COMMON,
            nullptr,
            IID_PPV_ARGS(&buff_)) >> utl::DxCheck;


    D3D12_SUBRESOURCE_DATA sub_resource_data = {};
    sub_resource_data.pData = info.data;
    sub_resource_data.RowPitch = size_;
    sub_resource_data.SlicePitch = sub_resource_data.RowPitch;

    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(buff_,
                                                        D3D12_RESOURCE_STATE_COMMON,
                                                        D3D12_RESOURCE_STATE_COPY_DEST);
    info.cmd_list->ResourceBarrier(1, &barrier);
    upload_buffer.copyData(0, (T *)info.data, info.count); //Copying cpu info to upload buvffer
    info.cmd_list->CopyResource(buff_, upload_buffer.get()); // Coping upload buffer into buffer

    barrier = CD3DX12_RESOURCE_BARRIER::Transition(buff_,
                                                   D3D12_RESOURCE_STATE_COPY_DEST,
                                                   D3D12_RESOURCE_STATE_GENERIC_READ);
    info.cmd_list->ResourceBarrier(1, &barrier);

    upload_buffer.release();
    setView(info);
    std::wstring name = L"Buffer" + std::to_wstring(mycounter++);
    buff_->SetName(name.c_str());
}

template<>
inline void Buffer<D3D12_VERTEX_BUFFER_VIEW, render::Vertex>::setView(BufferInitInfo &info) {
    view_.BufferLocation = buff_->GetGPUVirtualAddress();
    view_.SizeInBytes = size_;
    view_.StrideInBytes = sizeof(render::Vertex);
}

template<>
inline void Buffer<D3D12_INDEX_BUFFER_VIEW, u32>::setView(BufferInitInfo &info) {
    view_.BufferLocation = buff_->GetGPUVirtualAddress();
    view_.SizeInBytes = size_;
    view_.Format= info.format;
}

}