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

#include <array>

namespace reveal3d::graphics::dx12 {

struct DescriptorHandle {
  [[nodiscard]] bool IsValid() const { return cpu.ptr != 0; }

  [[nodiscard]] bool IsShaderVisible() const { return gpu.ptr != 0; }

  D3D12_CPU_DESCRIPTOR_HANDLE cpu {};
  D3D12_GPU_DESCRIPTOR_HANDLE gpu {};
  u32 index {};
};


// TODO: Concept for checking Descriptor type is D3D12_DESCRIPTOR_HEAP_TYPE
class DescriptorHeap {
public:
  explicit DescriptorHeap(const D3D12_DESCRIPTOR_HEAP_TYPE type) : type_(type) { }
  explicit DescriptorHeap(DescriptorHeap const&)   = delete;
  DescriptorHeap& operator=(DescriptorHeap const&) = delete;
  explicit DescriptorHeap(DescriptorHeap&&)        = delete;
  DescriptorHeap& operator=(DescriptorHeap&&)      = delete;
  ~DescriptorHeap()                                = default;

  // inline D3D12_DESCRIPTOR_HEAP_TYPE GetType() { return static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(Type); }
  [[nodiscard]] ID3D12DescriptorHeap* get() const { return heap_; };
  [[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE cpuStart() const { return cpu_start_; }
  [[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE gpuStart() const { return gpu_start_; }
  [[nodiscard]] u32 capacity() const { return capacity_; }
  [[nodiscard]] u32 size() const { return size_; }
  [[nodiscard]] u32 descriptorSize() const { return capacity_; }
  [[nodiscard]] bool isShaderVisible() const { return gpu_start_.ptr != 0; }

  bool initialize(ID3D12Device* device, u32 capacity, bool is_shader_visible);
  [[nodiscard]] DescriptorHandle alloc();
  void free(DescriptorHandle& handle);
  void cleanDeferreds();
  void release();

private:
  ID3D12DescriptorHeap* heap_ {nullptr};
  D3D12_CPU_DESCRIPTOR_HANDLE cpu_start_ {};
  D3D12_GPU_DESCRIPTOR_HANDLE gpu_start_ {};
  std::unique_ptr<u32[]> free_handles_ {};
  std::array<std::vector<u32>, config::render.graphics.max_buffer_count> deferred_indices_;
  u32 capacity_ {0};
  u32 size_ {0};
  u32 descriptor_size_ {};
  D3D12_DESCRIPTOR_HEAP_TYPE type_;

  // std::mutexmutex in future
};


struct Heaps {
  Heaps();
  void release();

  /******************** Descriptor Heaps *************************/
  DescriptorHeap rtv;
  DescriptorHeap dsv;
  DescriptorHeap srv;
  //    DescriptorHeap cbv;
  //    DescriptorHeap uavHeap; //TODO
};

} // namespace reveal3d::graphics::dx12
