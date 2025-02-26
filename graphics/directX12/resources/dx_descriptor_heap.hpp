/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_descriptor_heap.hpp
 * @version 1.0
 * @date 12/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "config/config.hpp"
#include "dx_deferring_system.hpp"

#include "dx_descriptor_heap_type.hpp"
#include "graphics/directX12/dx_adapter.hpp"
#include "graphics/directX12/dx_commands.hpp"

#include <array>
#include <unordered_map>

#include "dx_release_policies.hpp"

namespace reveal3d::graphics::dx12 {

template<HeapType Type>
class DescriptorHeap {
public:
  explicit DescriptorHeap(u32 const capacity) : free_handles_(std::move(std::make_unique<u32[]>(capacity))) {
    assert(capacity && capacity < D3D12_MAX_SHADER_VISIBLE_DESCRIPTOR_HEAP_SIZE_TIER_2);
    assert(
        !(static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(Type) == D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER &&
          capacity > D3D12_MAX_SHADER_VISIBLE_SAMPLER_HEAP_SIZE)
    );

    D3D12_DESCRIPTOR_HEAP_DESC const desc {
      .Type           = static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(Type),
      .NumDescriptors = capacity,
      .Flags    = is_shader_visible<Type> ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
      .NodeMask = 0,
    };

    release();

    if (FAILED(adapter.device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap_)))) {
      release();
      throw std::runtime_error("failed to create descriptor heap");
    }

    capacity_ = capacity;

    for (u32 i = 0; i < capacity; ++i) {
      free_handles_[i] = i;
    }

    // DEBUG_ACTION(for (u32 i{ 0 }; i < frameBufferCount; ++i) assert(_deferred_free_indices[i].empty()));
    descriptor_size_ = adapter.device->GetDescriptorHandleIncrementSize(static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(Type));
    cpu_start_       = heap_->GetCPUDescriptorHandleForHeapStart();
    gpu_start_ = is_shader_visible<Type> ? heap_->GetGPUDescriptorHandleForHeapStart() : D3D12_GPU_DESCRIPTOR_HANDLE {};
  }

  // Operador de asignación por movimiento
  DescriptorHeap& operator=(DescriptorHeap&& other) noexcept {
    if (this != &other) {
      heap_             = std::move(other.heap_);
      cpu_start_        = other.cpu_start_;
      gpu_start_        = other.gpu_start_;
      free_handles_     = std::move(other.free_handles_);
      deferred_indices_ = std::move(other.deferred_indices_);
      capacity_         = other.capacity_;
      size_             = other.size_;
      descriptor_size_  = other.descriptor_size_;
      other.capacity_   = 0;
      other.size_       = 0;
    }
    return *this;
  }

  DescriptorHeap(DescriptorHeap const&) = delete;

  DescriptorHeap& operator=(DescriptorHeap const&) = delete;

  ~DescriptorHeap() { release(); }

  [[nodiscard]] ID3D12DescriptorHeap* get() const { return heap_.Get(); }

  [[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE cpuStart() const { return cpu_start_; }

  [[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE gpuStart() const { return gpu_start_; }

  [[nodiscard]] u32 capacity() const { return capacity_; }

  [[nodiscard]] u32 size() const { return size_; }

  [[nodiscard]] u32 descriptorSize() const { return capacity_; }

  DescriptorHandle alloc() {
    assert(heap_);
    assert(size_ < capacity_);

    u32 const index {free_handles_[size_]};
    u32 const offset {index * descriptor_size_};
    ++size_;

    DescriptorHandle handle {
      .cpu =
          {
            .ptr = cpu_start_.ptr + offset,
          }, //
      .index = index
    };

    if constexpr (is_shader_visible<Type>) {
      handle.gpu.ptr = gpu_start_.ptr + offset;
    }

    return handle;
  }

  void deferredFree(DescriptorHandle const& handle) {
    assert(heap_ && size_);
    assert(handle.cpu.ptr >= cpu_start_.ptr);
    assert((handle.cpu.ptr - cpu_start_.ptr) % descriptor_size_ == 0);
    assert(handle.index < capacity_);

    u32 const frame_idx = Commands::frameIndex();
    deferred_indices_[frame_idx].push_back(handle.index);
    set_deferred_flag();
    // handle = {};
  }

  void free(DescriptorHandle const& handle) {
    --size_;
    free_handles_[size_] = handle.index;
  }

  void cleanDeferreds() {
    if (std::vector<u32> & indices {deferred_indices_[Commands::frameIndex()]}; !indices.empty()) {
      for (auto const index: indices) {
        // For cleaning Deferreds we just set them in free handles, so can be overwritten later
        --size_;
        free_handles_[size_] = index;
      }
      indices.clear();
    }
  }

  void release() const { deferred_release(heap_.Get()); }

private:
  ComPtr<ID3D12DescriptorHeap> heap_;
  D3D12_CPU_DESCRIPTOR_HANDLE cpu_start_ {};
  D3D12_GPU_DESCRIPTOR_HANDLE gpu_start_ {};
  std::unique_ptr<u32[]> free_handles_ {};
  std::array<std::vector<u32>, config::render.graphics.max_buffer_count> deferred_indices_ {};
  u32 capacity_ {0};
  u32 size_ {0};
  u32 descriptor_size_ {};
};


} // namespace reveal3d::graphics::dx12
