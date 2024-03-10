/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details) 
 ************************************************************************/
/**
 * @file dx_graphics.cpp
 * @version 1.0
 * @date 25/02/2024
 * @brief Short description
 *
 * Longer description
 */

#include "dx_graphics.hpp"
#include "dx_utils.hpp"

#include "window/window.hpp"
#include <iostream>

namespace reveal3d::graphics::Dx {

Graphics::Graphics(const window::Resolution &res) :
    rtvDescriptorSize_(0),
    fenceValues_{},
    res(res)
{
}

void Graphics::LoadPipeline() {
    // Factory -> LookForAdapter -> CreateDevice -> CommandQueue -> SwapChain
    InitDXGIAdapter();
    CreateCommandQueue();
    CreateSwapChain();
    InitFrameResources();
}

void Graphics::LoadAssets(){
    device_->CreateCommandList( 0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocators_[frameIndex_].Get(),
                                nullptr, IID_PPV_ARGS(&commandList_)) >> DxCheck;
    commandList_->Close();

    InitFence();
    WaitForGPU();
}

void Graphics::Update(render::Camera &camera) {

}

//TODO: Functions to simplify barrier creation
void Graphics::PopulateCommands() {
    auto &currentBuffer = renderTargets_[frameIndex_];
    commandAllocators_[frameIndex_]->Reset() >> DxCheck;
    commandList_->Reset(commandAllocators_[frameIndex_].Get(), nullptr) >> DxCheck; //TODO: Add Pipeline state

    auto targetBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
            currentBuffer.Get(),
            D3D12_RESOURCE_STATE_PRESENT,
            D3D12_RESOURCE_STATE_RENDER_TARGET );
    commandList_->ResourceBarrier(1, &targetBarrier);

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHeap_->GetCPUDescriptorHandleForHeapStart(), frameIndex_, rtvDescriptorSize_);

    const f32 clearColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    commandList_->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

    auto presentBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
            currentBuffer.Get(),
            D3D12_RESOURCE_STATE_RENDER_TARGET,
                    D3D12_RESOURCE_STATE_PRESENT
    );
    commandList_->ResourceBarrier(1, &presentBarrier);

    commandList_->Close() >> DxCheck;
}

void Graphics::Draw() {
    ID3D12CommandList* ppCommandLists[] = { commandList_.Get() };

    commandQueue_->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
    swapChain_->Present(0, 0) >> DxCheck;
}

void Graphics::MoveToNextFrame() {
    const u64 currentFenceVal = fenceValues_[frameIndex_];

    commandQueue_->Signal(fence_.Get(), currentFenceVal) >> DxCheck;

    frameIndex_ = swapChain_->GetCurrentBackBufferIndex();

    if (fence_->GetCompletedValue() < fenceValues_[frameIndex_])
    {
        fence_->SetEventOnCompletion(fenceValues_[frameIndex_], fenceEvent_) >> DxCheck;
        if (WaitForSingleObject(fenceEvent_, INFINITE) == WAIT_FAILED) {
            GetLastError() >> DxCheck;
        }
    }

    fenceValues_[frameIndex_] = currentFenceVal + 1;
}

void Graphics::WaitForGPU() {
    commandQueue_->Signal(fence_.Get(), fenceValues_[frameIndex_]) >> DxCheck;
    fence_->SetEventOnCompletion(fenceValues_[frameIndex_]++, fenceEvent_) >> DxCheck;
    if (WaitForSingleObject(fenceEvent_, INFINITE) == WAIT_FAILED) {
        GetLastError() >> DxCheck;
    }
}



//TODO: search for first avaible hardware adapter and look for best performance adapter (GPU)
//TODO: check for features
//TODO: add Info Queue
void Graphics::InitDXGIAdapter() {
    u32 factoryFlags = 0;

#ifdef _DEBUG
    Debugger::Enable(factoryFlags);
#endif

    CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&factory_)) >> DxCheck;
    D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device_)) >> DxCheck;
}

void Graphics::CreateCommandQueue() {
    const D3D12_COMMAND_QUEUE_DESC queueDesc {
        .Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
        .Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
        .Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
        .NodeMask = 0
    };
    device_->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue_)) >> DxCheck;
}

void Graphics::CreateSwapChain() {
    ComPtr<IDXGISwapChain1> swapChain1;
    const DXGI_SWAP_CHAIN_DESC1 swapChainDesc {
            .Width = res.width,
            .Height = res.height,
            .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
            .Stereo = FALSE,
            .SampleDesc = { 1, 0 },
            .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
            .BufferCount = bufferCount_,
            .Scaling = DXGI_SCALING_STRETCH,
            .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
            .AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
            .Flags = 0, //TODO: Check feature DXGI_SWWAP_CHAIN_FLAG_ALLOW_TEARING
    };
    factory_->CreateSwapChainForHwnd(
            commandQueue_.Get(),
            window_,
            &swapChainDesc,
            nullptr,
            nullptr,
            &swapChain1
            );
    swapChain1.As(&swapChain_) >> DxCheck;
    frameIndex_ = swapChain_->GetCurrentBackBufferIndex();
}

void Graphics::InitFrameResources() {
    const D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {
            .Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
            .NumDescriptors = bufferCount_,
    };

    device_->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeap_)) >> DxCheck;
    rtvDescriptorSize_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHeap_->GetCPUDescriptorHandleForHeapStart());
    for(u32 i = 0; i < bufferCount_; ++i) {
       swapChain_->GetBuffer(i, IID_PPV_ARGS(&renderTargets_[i])) >> DxCheck;
       device_->CreateRenderTargetView(renderTargets_[i].Get(), nullptr, rtvHandle);
       rtvHandle.Offset(rtvDescriptorSize_);
       device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocators_[i])) >> DxCheck;
    }
}

void Graphics::InitFence() {
    device_->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_)) >> DxCheck;
    fenceEvent_ = CreateEventW(nullptr, FALSE, FALSE, nullptr);
    if (!fenceEvent_) {
        GetLastError() >> DxCheck;
        throw std::runtime_error("Failed to create fence event");
    }
}
void Graphics::Terminate() {
    WaitForGPU();
    CloseHandle(fenceEvent_);
}


} // namespace reveal3d::graphics
