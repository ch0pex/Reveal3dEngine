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


template<typename T>
struct alignas(256) Constant {
  Constant() : data() { }
  T data;
};

namespace detail {

template<typename T>
struct is_constant : std::false_type { };

template<typename T>
struct is_constant<Constant<T>> : std::true_type { };

} // namespace detail

template<typename T>
concept is_constant = detail::is_constant<T>::value;

/**
 * Upload buffers need lifetime extension
 * during a frame so uses delayed destruction
 *
 * @note this means ComPtr<ID3D12Resource> can't be used
 * @tparam T Data type to upload
 */
template<typename T>
class UploadBuffer {
public:
  // *** Type Traits
  using value_type = T;
  using iterator   = typename std::span<T>::iterator;

  explicit UploadBuffer(u64 const count) {
    auto const heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    auto const res_desc        = CD3DX12_RESOURCE_DESC::Buffer(count * sizeof(T));

    adapter.device->CreateCommittedResource(
        &heap_properties, D3D12_HEAP_FLAG_NONE, &res_desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
        IID_PPV_ARGS(&buff_)
    ) >> utl::DxCheck;

    T* data {nullptr};

    buff_->Map(0, nullptr, reinterpret_cast<void**>(&data)) >> utl::DxCheck;
    mapped_data_ = std::span<T>(data, data + count);
  }

  explicit UploadBuffer(UploadBuffer const&) = delete;

  ~UploadBuffer() {
    buff_->Unmap(0, nullptr);
    if constexpr (is_constant<value_type>) {
      buff_->Release();
    }
    else {
      deferred_release(buff_);
    }
  }

  UploadBuffer& operator=(UploadBuffer const&) = delete;

  explicit UploadBuffer(UploadBuffer&&) = delete;

  UploadBuffer& operator=(UploadBuffer&&) = delete;

  [[nodiscard]] ID3D12Resource* get() const { return buff_; }

  [[nodiscard]] u32 size() const { return mapped_data_.size(); }

  [[nodiscard]] D3D12_GPU_VIRTUAL_ADDRESS gpuStart() const { return buff_->GetGPUVirtualAddress(); }

  [[nodiscard]] D3D12_GPU_VIRTUAL_ADDRESS gpuPos(u32 const index) const {
    return buff_->GetGPUVirtualAddress() + (index * sizeof(T));
  }

  DescriptorHandle view(u64 const idx, DescriptorHeap& heap) const {
    u64 const buff_address                     = gpuStart() + (sizeof(T) * idx);
    const D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {.BufferLocation = buff_address, .SizeInBytes = sizeof(T)};

    DescriptorHandle const handle = heap.alloc();
    adapter.device->CreateConstantBufferView(&desc, handle.cpu);
    return handle;
  }

  [[nodiscard]] decltype(auto) at(u64 idx) {
    if constexpr (is_constant<value_type>) {
      return (mapped_data_[idx].data);
    }
    else {
      return (mapped_data_[idx]);
    }
  }

  [[nodiscard]] typename std::span<T>::iterator begin() const { return mapped_data_.begin(); }

  [[nodiscard]] typename std::span<T>::iterator end() const { return mapped_data_.end(); }

private:
  std::span<T> mapped_data_ {};
  ID3D12Resource* buff_ {};
};

template<typename T>
using ConstantBuffer = UploadBuffer<Constant<T>>;


} // namespace reveal3d::graphics::dx12
