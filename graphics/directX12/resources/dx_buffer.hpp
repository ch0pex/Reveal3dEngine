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
#include "render/vertex.hpp"
#include "utils/dx_checker.hpp"
#include "utils/dx_debug.hpp"
#include "window/window_info.hpp"

#include "d3dx12.h"
#include "dx_release_policies.hpp"


namespace reveal3d::graphics::dx12 {

struct BufferDescriptor {
  D3D12_RESOURCE_DESC res_desc {};
  D3D12_RESOURCE_STATES res_state {D3D12_RESOURCE_STATE_COMMON};
  D3D12_HEAP_PROPERTIES heap_properties {CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT)};
  std::optional<D3D12_CLEAR_VALUE> clear_value {std::nullopt};
};

template<typename Policy = policy::Hard>
class Buffer {
public:
  using init_info = BufferDescriptor;

  explicit Buffer(init_info const& info) : size_(info.res_desc.Width * info.res_desc.Height) {
    auto const* opt_clear = info.clear_value.has_value() ? &info.clear_value.value() : nullptr;

    adapter.device->CreateCommittedResource(
        &info.heap_properties, D3D12_HEAP_FLAG_NONE, &info.res_desc, info.res_state, opt_clear, IID_PPV_ARGS(&buff_)
    ) >> utils::DxCheck;

    std::wstring const name = L"Buffer " + std::to_wstring(counter++);
    buff_->SetName(name.c_str()) >> utils::DxCheck;
    logger(LogInfo) << "Allocated buffer with size: " << size_;
  }

  Buffer(Buffer&& other) noexcept : buff_(other.buff_), size_(other.size_) { other.reset(); }

  Buffer(Buffer& other) = delete;

  ~Buffer() { Policy::release(buff_); }

  Buffer& operator=(Buffer& other) = delete;

  Buffer& operator=(Buffer&& other) noexcept {
    deferred_release(buff_);
    buff_ = other.buff_;
    size_ = other.size_;
    other.reset();
    return *this;
  }

  [[nodiscard]] ID3D12Resource* resource() const { return buff_; }

  [[nodiscard]] u32 size() const { return size_; };

  [[nodiscard]] D3D12_GPU_VIRTUAL_ADDRESS gpuAddress() const { return buff_->GetGPUVirtualAddress(); }

private:
  void reset() {
    buff_ = nullptr;
    size_ = 0;
  }

  static inline u32 counter = 0;
  ID3D12Resource* buff_ {};
  u32 size_;
};

inline D3D12_VERTEX_BUFFER_VIEW vertex_view(Buffer<policy::Deferred> const& buffer) {
  return {
    .BufferLocation = buffer.gpuAddress(),
    .SizeInBytes    = buffer.size(),
    .StrideInBytes  = sizeof(render::Vertex),
  };
}

inline D3D12_INDEX_BUFFER_VIEW index_view(Buffer<policy::Deferred> const& buffer) {
  return {
    .BufferLocation = buffer.gpuAddress(),
    .SizeInBytes    = buffer.size(),
    .Format         = DXGI_FORMAT_R32_UINT,
  };
}

} // namespace reveal3d::graphics::dx12
