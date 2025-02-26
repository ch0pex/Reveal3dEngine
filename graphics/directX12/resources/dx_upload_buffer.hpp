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

#include "d3dx12.h"
#include "dx_buffer.hpp"
#include "dx_descriptor_heap.hpp"
#include "graphics/directX12/dx_adapter.hpp"

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
 * Upload buffers need lifetime extension during a frame
 * so uses delayed destruction
 *
 * @note this means ComPtr<ID3D12Resource> can't be used
 * @tparam T Data type to upload
 */
template<typename T, typename Policy = policy::Deferred>
class UploadBuffer {
public:
  // *** Type Traits
  using value_type = T;
  using iterator   = typename std::span<T>::iterator;

  explicit UploadBuffer(u64 const count) {
    auto const [res_desc, res_state, heap_properties, clear_value] = BufferDescriptor {
      .res_desc        = CD3DX12_RESOURCE_DESC::Buffer(count * sizeof(T)),
      .heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
    };

    adapter.device->CreateCommittedResource(
        &heap_properties, D3D12_HEAP_FLAG_NONE, &res_desc, res_state, nullptr, IID_PPV_ARGS(&buff_)
    ) >> utils::DxCheck;
    T* data {nullptr};

    buff_->Map(0, nullptr, std::bit_cast<void**>(&data)) >> utils::DxCheck;
    mapped_data_ = std::span<T>(data, data + count);
    gpu_address_ = buff_->GetGPUVirtualAddress();
  }

  explicit UploadBuffer(UploadBuffer const&) = delete;

  ~UploadBuffer() {
    buff_->Unmap(0, nullptr);
    Policy::release(buff_);
  }

  UploadBuffer& operator=(UploadBuffer const&) = delete;

  explicit UploadBuffer(UploadBuffer&&) = delete;

  UploadBuffer& operator=(UploadBuffer&&) = delete;

  [[nodiscard]] ID3D12Resource* get() const { return buff_; }

  [[nodiscard]] u32 size() const { return mapped_data_.size(); }

  [[nodiscard]] D3D12_GPU_VIRTUAL_ADDRESS gpuStart() const { return gpu_address_; }

  [[nodiscard]] D3D12_GPU_VIRTUAL_ADDRESS gpuPos(u32 const index) const { return gpu_address_ + (index * sizeof(T)); }

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
  ID3D12Resource* buff_ {nullptr};
  D3D12_GPU_VIRTUAL_ADDRESS gpu_address_;
};

template<typename T>
using ConstantBuffer = UploadBuffer<Constant<T>, policy::Hard>;

/**
 * @name upload_resource
 * @param cmd_list command that will upload the data
 * @param buffer Gpu buffer where to copy the date
 * @param data data to upload
 */
template<typename T>
void upload_resource(ID3D12GraphicsCommandList* cmd_list, auto& buffer, std::span<T> data = {}) {
  auto upload_buffer = UploadBuffer<T>(data.size()); // Upload buffer is created in order to store buffer in gpu

  auto barrier = CD3DX12_RESOURCE_BARRIER::Transition( // Barrier, changing buffer state to copy dest
      buffer.resource(), //
      D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST
  );
  cmd_list->ResourceBarrier(1, &barrier);

  std::ranges::copy(data, upload_buffer.begin());
  cmd_list->CopyResource(buffer.resource(), upload_buffer.get()); // Coping upload buffer into buffer

  barrier = CD3DX12_RESOURCE_BARRIER::Transition( // Restoring buffer state to generic read
      buffer.resource(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ
  );
  cmd_list->ResourceBarrier(1, &barrier);
}

} // namespace reveal3d::graphics::dx12
