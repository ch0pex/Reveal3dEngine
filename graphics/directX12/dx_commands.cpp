/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_commands.cpp
 * @version 1.0
 * @date 12/03/2024
 * @brief Short description
 *
 * Longer description
 */

#include "dx_commands.hpp"
#include "dx_adapter.hpp"
#include "utils/dx_defaults.hpp"


namespace reveal3d::graphics::dx12 {

u8 Commands::frame_index_ = 0;

Commands::Commands() {
  resetFences();
  adapter.device->CreateCommandQueue(&defaults::queue_desc, IID_PPV_ARGS(&command_queue_)) >> utils::DxCheck;

  adapter.device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_)) >> utils::DxCheck;
  fence_event_ = CreateEventW(nullptr, FALSE, FALSE, nullptr);
  if (fence_event_ == nullptr) {
    GetLastError() >> utils::DxCheck;
    throw std::runtime_error("Failed to create fence event");
  }

  for (auto& command_allocator: command_allocators_) {
    adapter.device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&command_allocator)) >>
        utils::DxCheck;
  }

  adapter.device->CreateCommandList(
      0, D3D12_COMMAND_LIST_TYPE_DIRECT, command_allocators_[frame_index_].Get(), nullptr, IID_PPV_ARGS(&command_list_)
  ) >> utils::DxCheck;
  command_list_->Close() >> utils::DxCheck;
}

void Commands::reset(ID3D12PipelineState* pso) const {
  command_allocators_[frame_index_]->Reset() >> utils::DxCheck;
  command_list_->Reset(command_allocators_[frame_index_].Get(), pso) >> utils::DxCheck;
}

void Commands::waitForGpu() {
  fence_values_[frame_index_]++;
  command_queue_->Signal(fence_.Get(), fence_values_[frame_index_]) >> utils::DxCheck;

  if (fence_->GetCompletedValue() < fence_values_[frame_index_]) {
    fence_->SetEventOnCompletion(fence_values_[frame_index_], fence_event_) >> utils::DxCheck;
    if (WaitForSingleObject(fence_event_, INFINITE) == WAIT_FAILED) {
      GetLastError() >> utils::DxCheck;
    }
  }
}

void Commands::moveToNextFrame() {
  u64 const current_fence_val = fence_values_[frame_index_];

  command_queue_->Signal(fence_.Get(), current_fence_val) >> utils::DxCheck;

  frame_index_ = ++frame_index_ % config::render.graphics.buffer_count;

  if (fence_->GetCompletedValue() < fence_values_[frame_index_]) {
    fence_->SetEventOnCompletion(fence_values_[frame_index_], fence_event_) >> utils::DxCheck;
    if (WaitForSingleObject(fence_event_, INFINITE) == WAIT_FAILED) {
      GetLastError() >> utils::DxCheck;
    }
  }

  fence_values_[frame_index_] = current_fence_val + 1;
}
void Commands::execute() const {
  ID3D12CommandList* pp_command_lists[] = {command_list_.Get()};
  command_queue_->ExecuteCommandLists(_countof(pp_command_lists), pp_command_lists);
}
Commands::~Commands() {
  waitForGpu();
  CloseHandle(fence_event_);
}

void Commands::resetFences() {
  frame_index_ = 0;
  for (auto i = 1; i < config::render.graphics.buffer_count; i++) {
    fence_values_.at(i) = 0;
  }
}

void Commands::addGraphicsList(ID3D12GraphicsCommandList* list) { }


} // namespace reveal3d::graphics::dx12
