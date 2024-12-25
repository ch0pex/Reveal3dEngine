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


#include "dx_deferring_system.hpp"
#include "dx_upload_buffer.hpp"
#include "render/vertex.hpp"
#include "utils/dx_checker.hpp"
#include "utils/dx_debug.hpp"
#include "window/window_info.hpp"

#include "d3dx12.h"


namespace reveal3d::graphics::dx12 {

class Buffer {
public:
  struct InitInfo {
    D3D12_RESOURCE_DESC res_desc {};
    D3D12_RESOURCE_STATES res_state {D3D12_RESOURCE_STATE_COMMON};
    D3D12_HEAP_PROPERTIES heap_properties {CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT)};
    std::optional<D3D12_CLEAR_VALUE> clear_value {std::nullopt};
  };

  explicit Buffer(InitInfo const& info) : size_(info.res_desc.Width * info.res_desc.Height) {
    auto const* opt_clear = info.clear_value.has_value() ? &info.clear_value.value() : nullptr;

    adapter.device->CreateCommittedResource(
        &info.heap_properties, D3D12_HEAP_FLAG_NONE, &info.res_desc, info.res_state, opt_clear, IID_PPV_ARGS(&buff_)
    ) >> utl::DxCheck;

    gpu_address_            = buff_->GetGPUVirtualAddress();
    std::wstring const name = L"Buffer " + std::to_wstring(counter++);
    buff_->SetName(name.c_str()) >> utl::DxCheck;
  }

  ~Buffer() { dx12::release(buff_); }

  Buffer& operator=(Buffer&& other) noexcept {
    release();
    buff_       = other.buff_;
    size_       = other.size_;
    other.buff_ = nullptr;
    other.size_ = 0;
    return *this;
  }

  template<typename T>
  void upload(ID3D12GraphicsCommandList* cmd_list, std::span<T> data = {}) {
    auto upload_buffer = UploadBuffer<T>(data.size()); // Upload buffer is created in order to store buffer in gpu

    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition( // Barrier, changing buffer state to copy dest
        buff_, //
        D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST
    );
    cmd_list->ResourceBarrier(1, &barrier);

    std::ranges::copy(data, upload_buffer.begin());
    cmd_list->CopyResource(buff_, upload_buffer.get()); // Coping upload buffer into buffer

    barrier = CD3DX12_RESOURCE_BARRIER::Transition( // Restoring buffer state to generic read
        buff_, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ
    );
    cmd_list->ResourceBarrier(1, &barrier);
  }

  void release() const {
    logger(LogInfo) << "Releasing gpu memory buffer with size " << size_;
    deferred_release(buff_);
  }

  [[nodiscard]] ID3D12Resource* resource() const { return buff_; }

  [[nodiscard]] u32 size() const { return size_; };

  [[nodiscard]] D3D12_GPU_VIRTUAL_ADDRESS gpu_address() const { return gpu_address_; }

  static constexpr auto buffer1d = [](u64 const width, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN) {
    return InitInfo {
      .res_desc  = CD3DX12_RESOURCE_DESC::Buffer(width),
      .res_state = D3D12_RESOURCE_STATE_COMMON,
      .heap_properties {CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT)},
      .clear_value {std::nullopt},
    };
  };

private:
  static inline u32 counter = 0;
  D3D12_GPU_VIRTUAL_ADDRESS gpu_address_ {};
  ID3D12Resource* buff_ {};
  u32 size_ {0};
};

template<typename T>
T buffer_view(Buffer const& buffer);

template<>
inline D3D12_VERTEX_BUFFER_VIEW buffer_view<D3D12_VERTEX_BUFFER_VIEW>(Buffer const& buffer) {
  return {
    .BufferLocation = buffer.gpu_address(),
    .SizeInBytes    = buffer.size(),
    .StrideInBytes  = sizeof(render::Vertex),
  };
}

template<>
inline D3D12_INDEX_BUFFER_VIEW buffer_view<D3D12_INDEX_BUFFER_VIEW>(Buffer const& buffer) {
  return {
    .BufferLocation = buffer.gpu_address(),
    .SizeInBytes    = buffer.size(),
    .Format         = DXGI_FORMAT_R32_UINT,
  };
}

} // namespace reveal3d::graphics::dx12
