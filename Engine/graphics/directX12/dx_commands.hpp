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


namespace reveal3d::graphics::dx {

class Commands {
public:
    //explicit Commands(bufferCount);
    void Init(ID3D12Device* device);
    [[nodiscard]] inline ID3D12CommandQueue * GetQueue() const { return commandQueue_.Get(); }
    [[nodiscard]] inline ID3D12GraphicsCommandList * const GetList() const { return commandList_.Get(); }
    [[nodiscard]] static inline u8 const FrameIndex() { return frameIndex_; }
    void Reset();
    void Execute();

    void WaitForGPU();
    void MoveToNextFrame();

    void Terminate();

private:
    //This is right now the same render target buffers on the swap chain, but could be more
    //Makes sense to be the same
    //More adds delay to the app

    static const u32 bufferCount_ = 3;
    ComPtr<ID3D12CommandQueue> commandQueue_;
    ComPtr<ID3D12GraphicsCommandList> commandList_;
    ComPtr<ID3D12CommandAllocator> commandAllocators_[bufferCount_];

    // Synchronization objects.
    static u8 frameIndex_;
    HANDLE fenceEvent_;
    ComPtr<ID3D12Fence> fence_;
    u64 fenceValues_[bufferCount_] {0};
};

}



