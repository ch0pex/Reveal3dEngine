/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_descriptor_heap.cpp
 * @version 1.0
 * @date 12/03/2024
 * @brief Short description
 *
 * Longer description
 */

#include "dx_descriptor_heap.hpp"
#include "../dx_commands.hpp"

namespace reveal3d::graphics::dx12 {

bool DescriptorHeap::initialize(ID3D12Device* const device, u32 capacity, bool is_shader_visible) {

  assert(capacity && capacity < D3D12_MAX_SHADER_VISIBLE_DESCRIPTOR_HEAP_SIZE_TIER_2);
  assert(!(type_ == D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER && capacity > D3D12_MAX_SHADER_VISIBLE_SAMPLER_HEAP_SIZE));

  if (type_ == D3D12_DESCRIPTOR_HEAP_TYPE_DSV || type_ == D3D12_DESCRIPTOR_HEAP_TYPE_RTV) {
    is_shader_visible = false;
  }

  release();

  D3D12_DESCRIPTOR_HEAP_DESC desc {
    .Type           = type_,
    .NumDescriptors = capacity,
    .Flags          = is_shader_visible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
    .NodeMask       = 0,
  };

  if (FAILED(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap_)))) {
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
  descriptor_size_ = device->GetDescriptorHandleIncrementSize(type_);
  cpu_start_       = heap_->GetCPUDescriptorHandleForHeapStart();
  gpu_start_       = is_shader_visible ? heap_->GetGPUDescriptorHandleForHeapStart() : D3D12_GPU_DESCRIPTOR_HANDLE {0};

  return true;
}

DescriptorHandle DescriptorHeap::alloc() {
  assert(heap_);
  assert(size_ < capacity_);

  u32 const index {free_handles_[size_]};
  u32 const offset {index * descriptor_size_};
  ++size_;

  DescriptorHandle handle;
  handle.cpu.ptr = cpu_start_.ptr + offset;
  if (isShaderVisible()) {
    handle.gpu.ptr = gpu_start_.ptr + offset;
  }

  handle.index = index;
  // DEBUG_OP(backBufferHandle.container = this);
  return handle;
}

void DescriptorHeap::free(DescriptorHandle& handle) {
  if (!handle.IsValid())
    return;
  assert(heap_ && size_);
  assert(handle.cpu.ptr >= cpu_start_.ptr);
  assert((handle.cpu.ptr - cpu_start_.ptr) % descriptor_size_ == 0);
  assert(handle.index < capacity_);
  u32 const index {(u32)(handle.cpu.ptr - cpu_start_.ptr) / descriptor_size_};
  assert(handle.index == index);

  u32 const frame_idx = Commands::frameIndex();
  deferred_indices_[frame_idx].push_back(index);
  set_deferred_flag();
  handle = {};
}

void DescriptorHeap::cleanDeferreds() {
  std::vector<u32>& indices {deferred_indices_[Commands::frameIndex()]};
  if (!indices.empty()) {
    for (auto index: indices) {
      --size_;
      free_handles_[size_] = index; // For cleaning Deferreds we just set them in free handles
    }
    indices.clear();
  }
}

void DescriptorHeap::release() { deferred_release(heap_); }


Heaps::Heaps() :
  rtv(D3D12_DESCRIPTOR_HEAP_TYPE_RTV), dsv(D3D12_DESCRIPTOR_HEAP_TYPE_DSV), srv(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
//    cbv(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
//        uavHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
{ }
void Heaps::release() {
  rtv.release();
  dsv.release();
  srv.release();
  //    cbv.release();
  //    uavHeap.release();
}

} // namespace reveal3d::graphics::dx12
