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
#include "dx_upload_buffer.hpp"
#include "render/vertex.hpp"

namespace reveal3d::graphics::dx12 {

template<typename T>
struct BufferInitInfo {
  ID3D12Device* device {};
  ID3D12GraphicsCommandList* cmd_list {};
  std::span<T> view {};
  DXGI_FORMAT format {};
};

template<typename BUFFER_VIEW_TYPE, typename T>
class Buffer {
public:
  Buffer() = default;

  explicit Buffer(BufferInitInfo<T>& info) { init(info); }

  void init(BufferInitInfo<T>& info) {
    static u32 my_counter     = 0;
    count_                    = info.view.size();
    size_                     = sizeof(T) * info.view.size();
    auto upload_buffer        = UploadBuffer<T>(); // Upload buffer is created in order to store buffer in gpu
    auto const heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    auto const res_desc       = CD3DX12_RESOURCE_DESC::Buffer(size_);

    upload_buffer.init(info.device, info.view.size());

    info.device->CreateCommittedResource(
        &heapProperties, D3D12_HEAP_FLAG_NONE, &res_desc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&buff_)
    ) >> utl::DxCheck;

    // D3D12_SUBRESOURCE_DATA sub_resource_data = { .pData = info.view.data(), .RowPitch = size_, .SlicePitch = size_
    // };

    auto barrier =
        CD3DX12_RESOURCE_BARRIER::Transition(buff_, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
    info.cmd_list->ResourceBarrier(1, &barrier);
    upload_buffer.copyData(0, info.view.data(), info.view.size()); // Copying cpu info to upload buvffer
    info.cmd_list->CopyResource(buff_, upload_buffer.get()); // Coping upload buffer into buffer

    barrier =
        CD3DX12_RESOURCE_BARRIER::Transition(buff_, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
    info.cmd_list->ResourceBarrier(1, &barrier);

    upload_buffer.release();
    setView(info);
    std::wstring const name = L"Buffer" + std::to_wstring(my_counter++);
    buff_->SetName(name.c_str()) >> utl::DxCheck;
  }

  void release() const { deferred_release(buff_); }

  [[nodiscard]] ID3D12Resource* Get() const { return buff_; };

  [[nodiscard]] BUFFER_VIEW_TYPE* view() { return &view_; };

  [[nodiscard]] u32 count() const { return count_; };

  [[nodiscard]] u32 SizeInBytes() const { return size_; };

private:
  void setView(BufferInitInfo<T> const& info);

  BUFFER_VIEW_TYPE view_;
  ID3D12Resource* buff_ {};
  u32 size_ {0};
  u32 count_ {0};
};

template<>
inline void Buffer<D3D12_VERTEX_BUFFER_VIEW, render::Vertex>::setView(BufferInitInfo<render::Vertex> const& info) {
  view_.BufferLocation = buff_->GetGPUVirtualAddress();
  view_.SizeInBytes    = size_;
  view_.StrideInBytes  = sizeof(render::Vertex);
}

template<>
inline void Buffer<D3D12_INDEX_BUFFER_VIEW, u32>::setView(BufferInitInfo<u32> const& info) {
  view_.BufferLocation = buff_->GetGPUVirtualAddress();
  view_.SizeInBytes    = size_;
  view_.Format         = info.format;
}

using VertexBuffer = Buffer<D3D12_VERTEX_BUFFER_VIEW, render::Vertex>;
using IndexBuffer  = Buffer<D3D12_INDEX_BUFFER_VIEW, u32>;

} // namespace reveal3d::graphics::dx12
