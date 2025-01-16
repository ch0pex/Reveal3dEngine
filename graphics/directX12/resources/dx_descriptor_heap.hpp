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

#include "concepts.hpp"
#include "config/config.hpp"
#include "dx_deferring_system.hpp"

#include <array>

#include "dx_descriptor_heap_type.hpp"
#include "graphics/directX12/dx_adapter.hpp"
#include "graphics/directX12/dx_commands.hpp"

namespace reveal3d::graphics::dx12 {

template<HeapType Type>
class DescriptorHeap {
public:
  explicit DescriptorHeap(u32 const capacity) { initialize(capacity); }

  explicit DescriptorHeap(DescriptorHeap const&) = delete;

  DescriptorHeap& operator=(DescriptorHeap const&) = delete;

  explicit DescriptorHeap(DescriptorHeap&&) = delete;

  DescriptorHeap& operator=(DescriptorHeap&&) = delete;

  [[nodiscard]] ID3D12DescriptorHeap* get() const { return heap_.Get(); }

  [[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE cpuStart() const { return cpu_start_; }

  [[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE gpuStart() const { return gpu_start_; }

  [[nodiscard]] u32 capacity() const { return capacity_; }

  [[nodiscard]] u32 size() const { return size_; }

  [[nodiscard]] u32 descriptorSize() const { return capacity_; }

  bool initialize(u32 const capacity) {

    assert(capacity && capacity < D3D12_MAX_SHADER_VISIBLE_DESCRIPTOR_HEAP_SIZE_TIER_2);
    assert(
        !(static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(Type) == D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER &&
          capacity > D3D12_MAX_SHADER_VISIBLE_SAMPLER_HEAP_SIZE)
    );


    release();

    D3D12_DESCRIPTOR_HEAP_DESC const desc {
      .Type           = static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(Type),
      .NumDescriptors = capacity,
      .Flags    = is_shader_visible<Type> ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
      .NodeMask = 0,
    };

    if (FAILED(adapter.device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap_)))) {
      release();
      return false;
    }

    free_handles_ = std::move(std::make_unique<u32[]>(capacity));
    capacity_     = capacity;
    size_         = 0;

    for (u32 i = 0; i < capacity; ++i) {
      free_handles_[i] = i;
    }

    // DEBUG_ACTION(for (u32 i{ 0 }; i < frameBufferCount; ++i) assert(_deferred_free_indices[i].empty()));
    descriptor_size_ = adapter.device->GetDescriptorHandleIncrementSize(static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(Type));
    cpu_start_       = heap_->GetCPUDescriptorHandleForHeapStart();
    gpu_start_ =
        is_shader_visible<Type> ? heap_->GetGPUDescriptorHandleForHeapStart() : D3D12_GPU_DESCRIPTOR_HANDLE {0};

    return true;
  }

  template<resource T, typename... Args>
  T alloc(auto const& init_info, Args... args) {
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

    return T {handle, init_info, args...};
  }

  template<resource T>
  void free(T& resource) {
    free_handle(resource.handle());
    deferred_release(resource.resource());
  }

  void cleanDeferreds() {
    if (std::vector<u32> & indices {deferred_indices_[Commands::frameIndex()]}; !indices.empty()) {
      for (auto const index: indices) {
        --size_;
        free_handles_[size_] = index; // For cleaning Deferreds we just set them in free handles
      }
      indices.clear();
    }
  }

  void release() const { deferred_release(heap_.Get()); }

private:
  void free_handle(DescriptorHandle& handle) {
    assert(heap_ && size_);
    assert(handle.cpu.ptr >= cpu_start_.ptr);
    assert((handle.cpu.ptr - cpu_start_.ptr) % descriptor_size_ == 0);
    assert(handle.index < capacity_);

    u32 const index {static_cast<u32>(handle.cpu.ptr - cpu_start_.ptr) / descriptor_size_};
    assert(handle.index == index);

    u32 const frame_idx = Commands::frameIndex();
    deferred_indices_[frame_idx].push_back(index);
    set_deferred_flag();
    handle = {};
  }

  ComPtr<ID3D12DescriptorHeap> heap_;
  D3D12_CPU_DESCRIPTOR_HANDLE cpu_start_ {};
  D3D12_GPU_DESCRIPTOR_HANDLE gpu_start_ {};
  std::unique_ptr<u32[]> free_handles_ {};
  std::array<std::vector<u32>, config::render.graphics.max_buffer_count> deferred_indices_;
  u32 capacity_ {0};
  u32 size_ {0};
  u32 descriptor_size_ {};
};


struct Heaps {
  Heaps() : rtv(config::render.graphics.buffer_count), dsv(1U), srv(1U) { }

  ~Heaps() { logger(LogInfo) << "Releasing gpu heaps"; }

  void cleanDeferreds() {
    rtv.cleanDeferreds();
    dsv.cleanDeferreds();
    srv.cleanDeferreds();
    //  uavHeap.cleanDeferreds();
    //  uavHeap.cleanDeferreds();
  }

  void release();

  /******************** Descriptor Heaps *************************/
  DescriptorHeap<HeapType::Rtv> rtv;
  DescriptorHeap<HeapType::Dsv> dsv;
  DescriptorHeap<HeapType::Srv> srv;
  //    DescriptorHeap cbv;
  //    DescriptorHeap uavHeap; //TODO
};


} // namespace reveal3d::graphics::dx12
