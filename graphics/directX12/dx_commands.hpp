/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_commands.hpp
 * @version 1.0
 * @date 12/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "core/config/config.hpp"
#include "dx_common.hpp"


namespace reveal3d::graphics::dx12 {

class Commands {
public:
  // explicit Commands(bufferCount);
  void init(ID3D12Device* device);
  [[nodiscard]] ID3D12CommandQueue* getQueue() const { return command_queue_.Get(); }
  [[nodiscard]] ID3D12GraphicsCommandList* list() const { return command_list_.Get(); }
  [[nodiscard]] static u8 frameIndex() { return frame_index_; }
  void reset(ID3D12PipelineState* pso = nullptr) const;
  void resetFences();
  void execute() const;

  void waitForGpu();
  void moveToNextFrame();

  void flush();
  void addGraphicsList(ID3D12GraphicsCommandList* list);

private:
  ComPtr<ID3D12CommandQueue> command_queue_;
  ComPtr<ID3D12GraphicsCommandList> command_list_;
  std::array<ComPtr<ID3D12CommandAllocator>, config::Graphics::max_buffer_count> command_allocators_;

  // Synchronization objects.
  static u8 frame_index_;
  HANDLE fence_event_ {};
  ComPtr<ID3D12Fence> fence_;
  std::array<u64, config::Graphics::max_buffer_count> fence_values_ {};
};

} // namespace reveal3d::graphics::dx12
