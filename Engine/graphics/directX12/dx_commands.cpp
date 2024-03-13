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
#include "dx_utils.hpp"


namespace reveal3d::graphics::dx {

u8 Commands::frameIndex_ = 0;

void Commands::Init(ID3D12Device *device) {
    const D3D12_COMMAND_QUEUE_DESC queueDesc {
            .Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
            .Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
            .Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
            .NodeMask = 0
    };
    device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue_)) >> utl::DxCheck;

    device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_)) >> utl::DxCheck;
    fenceEvent_ = CreateEventW(nullptr, FALSE, FALSE, nullptr);
    if (fenceEvent_ == nullptr) {
        GetLastError() >> utl::DxCheck;
        throw std::runtime_error("Failed to create fence event");
    }

    for(auto & commandAllocator : commandAllocators_) {
        device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator)) >> utl::DxCheck;
    }

    device->CreateCommandList( 0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocators_[frameIndex_].Get(),
                               nullptr, IID_PPV_ARGS(&commandList_)) >> utl::DxCheck;
    commandList_->Close();
}

void Commands::Reset() {
    commandAllocators_[frameIndex_]->Reset() >> utl::DxCheck;
    commandList_->Reset(commandAllocators_[frameIndex_].Get(), nullptr) >> utl::DxCheck; //TODO: Add Pipeline state
}

void Commands::WaitForGPU() {
    commandQueue_->Signal(fence_.Get(), fenceValues_[frameIndex_]) >> utl::DxCheck;
    fence_->SetEventOnCompletion(fenceValues_[frameIndex_]++, fenceEvent_) >> utl::DxCheck;
    if (WaitForSingleObject(fenceEvent_, INFINITE) == WAIT_FAILED) {
        GetLastError() >> utl::DxCheck;
    }

}

void Commands::MoveToNextFrame() {
    const u64 currentFenceVal = fenceValues_[frameIndex_];

    commandQueue_->Signal(fence_.Get(), currentFenceVal) >> utl::DxCheck;

    frameIndex_ = ++frameIndex_ % bufferCount_;

    if (fence_->GetCompletedValue() < fenceValues_[frameIndex_])
    {
        fence_->SetEventOnCompletion(fenceValues_[frameIndex_], fenceEvent_) >> utl::DxCheck;
        if (WaitForSingleObject(fenceEvent_, INFINITE) == WAIT_FAILED) {
            GetLastError() >> utl::DxCheck;
        }
    }

    fenceValues_[frameIndex_] = currentFenceVal + 1;
}
void Commands::Execute() {
    ID3D12CommandList* ppCommandLists[] = { commandList_.Get() };
    commandQueue_->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
}
void Commands::Flush() {
    WaitForGPU();
    CloseHandle(fenceEvent_);
}

}