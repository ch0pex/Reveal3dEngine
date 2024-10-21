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

#include "dx_common.hpp"


namespace reveal3d::graphics::dx12 {

class Commands {
public:
    //explicit Commands(bufferCount);
    void init(ID3D12Device *device);
    [[nodiscard]] inline ID3D12CommandQueue *getQueue() const { return command_queue_.Get(); }
    [[nodiscard]] inline ID3D12GraphicsCommandList *list() const { return command_list_.Get(); }
    [[nodiscard]] static inline u8 const frameIndex() { return frame_index_; }
    void reset(ID3D12PipelineState *pso= nullptr);
    void resetFences();
    void execute();

    void waitForGpu();
    void moveToNextFrame();

    void flush();
    void addGraphicsList(ID3D12GraphicsCommandList *list);

private:
    static const u32 buffer_count_ = 3;
    ComPtr<ID3D12CommandQueue> command_queue_;
    ComPtr<ID3D12GraphicsCommandList> command_list_;
    ComPtr<ID3D12CommandAllocator> command_allocators_[buffer_count_];

    // Synchronization objects.
    static u8 frame_index_;
    HANDLE fence_event_;
    ComPtr<ID3D12Fence> fence_;
    u64 fence_values_[buffer_count_] {0};
};

}



