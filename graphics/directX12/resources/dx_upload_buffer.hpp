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

#include "../dx_common.hpp"
#include "dx_descriptor_heap.hpp"

namespace reveal3d::graphics::dx12 {


// All constant items in constant buffers must be aligned with 256 bytes
template<typename T>
struct alignas(256) Constant {
  Constant() : data() { }
  T data;
};

struct Constant2 {

  u32 data;
};

// TODO: Improve Upload buffer and buffer to dynamic like heaps
template<typename T>
class UploadBuffer {
public:
  UploadBuffer() = default;

  explicit UploadBuffer(UploadBuffer const&) = delete;

  UploadBuffer& operator=(UploadBuffer const&) = delete;

  explicit UploadBuffer(UploadBuffer&&) = delete;

  UploadBuffer& operator=(UploadBuffer&&) = delete;

  void init(ID3D12Device* device, u32 count);

  [[nodiscard]] ID3D12Resource* get() const { return buff_; };

  [[nodiscard]] u32 size() const { return capacity_; };

  [[nodiscard]] D3D12_GPU_VIRTUAL_ADDRESS gpuStart() const { return buff_->GetGPUVirtualAddress(); }

  [[nodiscard]] D3D12_GPU_VIRTUAL_ADDRESS gpuPos(u32 const index) const {
    return buff_->GetGPUVirtualAddress() + (index * sizeof(T));
  }

  DescriptorHandle createView(ID3D12Device* device, DescriptorHeap& heap) {
    u64 const buff_address                     = gpuStart() + (sizeof(T) * size_++);
    const D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {.BufferLocation = buff_address, .SizeInBytes = sizeof(T)};

    DescriptorHandle const handle = heap.alloc();
    device->CreateConstantBufferView(&desc, handle.cpu);
    return handle;
  }

  void copyData(u32 element_index, T const* data, u32 const count = 1) {
    memcpy(&mapped_data_[element_index], data, sizeof(T) * count);
  }

  void release() const {
    if (buff_ != nullptr)
      buff_->Unmap(0, nullptr);
    deferred_release(buff_);
  }

private:
  T* mapped_data_ {nullptr};
  ID3D12Resource* buff_;
  u32 capacity_ {0};
  u32 size_ {0};
};


template<typename T>
void UploadBuffer<T>::init(ID3D12Device* device, u32 const count) {
  capacity_                  = sizeof(T) * count;
  auto const heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
  auto const res_desc        = CD3DX12_RESOURCE_DESC::Buffer(capacity_);

  device->CreateCommittedResource(
      &heap_properties, D3D12_HEAP_FLAG_NONE, &res_desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
      IID_PPV_ARGS(&buff_)
  ) >> utl::DxCheck;

  // TODO: addId option to hide this to cpu with range(0,0)
  buff_->Map(0, nullptr, reinterpret_cast<void**>(&mapped_data_));
}

} // namespace reveal3d::graphics::dx12
