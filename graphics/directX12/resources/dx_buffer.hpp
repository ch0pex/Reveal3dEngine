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


class Buffer {
public:
  Buffer() = default;

  struct InitInfo {

    ID3D12GraphicsCommandList* cmd_list;
    D3D12_RESOURCE_DESC res_desc;
    D3D12_RESOURCE_STATES res_state {D3D12_RESOURCE_STATE_COMMON};
    D3D12_HEAP_PROPERTIES heap_properties {CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT)};
    std::optional<D3D12_CLEAR_VALUE> clear_value {std::nullopt};
  };

  template<typename T>
  explicit Buffer(InitInfo& info, std::span<T> data = {}) {
    init(info);
  }

  template<typename T = void*>
  void init(InitInfo const& info, std::span<T> data = {}) {
    size_                 = info.res_desc.Width * info.res_desc.Height;
    auto const* opt_clear = info.clear_value.has_value() ? &info.clear_value.value() : nullptr;

    adapter.device->CreateCommittedResource(
        &info.heap_properties, D3D12_HEAP_FLAG_NONE, &info.res_desc, info.res_state, opt_clear, IID_PPV_ARGS(&buff_)
    ) >> utl::DxCheck;

    // D3D12_SUBRESOURCE_DATA sub_resource_data = { .pData = info.view.data(), .RowPitch = size_, .SlicePitch = size_
    // };

    if constexpr (not std::same_as<T, void*>) {
      auto upload_buffer = UploadBuffer<T>(data.size()); // Upload buffer is created in order to store buffer in gpu
      auto barrier =
          CD3DX12_RESOURCE_BARRIER::Transition(buff_, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
      info.cmd_list->ResourceBarrier(1, &barrier);
      upload_buffer.copyData(0, data.data(), data.size()); // Copying cpu info to upload buffer
      info.cmd_list->CopyResource(buff_, upload_buffer.get()); // Coping upload buffer into buffer

      barrier = CD3DX12_RESOURCE_BARRIER::Transition(
          buff_, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ
      );
      info.cmd_list->ResourceBarrier(1, &barrier);

      upload_buffer.release();
    }

    std::wstring const name = L"Buffer" + std::to_wstring(counter++);
    buff_->SetName(name.c_str()) >> utl::DxCheck;
  }

  void release() const { deferred_release(buff_); }

  ID3D12Resource*& resource() { return buff_; }

  [[nodiscard]] u32 size() const { return size_; };

  [[nodiscard]] D3D12_GPU_VIRTUAL_ADDRESS gpu_address() const { return buff_->GetGPUVirtualAddress(); }

  template<typename T>
  T view();

  static constexpr auto buffer1d = [](ID3D12GraphicsCommandList* cmd_lists, u64 const width,
                                      DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN) {
    return InitInfo {
      .cmd_list = cmd_lists,
      .res_desc = CD3DX12_RESOURCE_DESC::Buffer(width),
    };
  };

private:
  static inline u32 counter = 0;
  ID3D12Resource* buff_ {};
  u32 size_ {0};
};

template<>
inline D3D12_VERTEX_BUFFER_VIEW Buffer::view<D3D12_VERTEX_BUFFER_VIEW>() {
  return {
    .BufferLocation = buff_->GetGPUVirtualAddress(),
    .SizeInBytes    = size_,
    .StrideInBytes  = sizeof(render::Vertex),
  };
}

template<>
inline D3D12_INDEX_BUFFER_VIEW Buffer::view<D3D12_INDEX_BUFFER_VIEW>() {
  return {
    .BufferLocation = buff_->GetGPUVirtualAddress(),
    .SizeInBytes    = size_,
    .Format         = DXGI_FORMAT_R32_UINT,
  };
}

} // namespace reveal3d::graphics::dx12
