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
#include "render/mesh.hpp"

namespace reveal3d::graphics::dx {

using namespace render;

Graphics::Graphics(const window::Resolution &res) :
    resolution_(res), presentInfo_(0), allowTearing_(0)

{}

void Graphics::LoadPipeline() {
    // Factory -> LookForAdapter -> CreateDevice -> CommandQueue -> SwapChain
    InitDXGIAdapter();
    cmdManager_.Init(device_.Get());
    heaps_.rtv.Initialize(device_.Get(), 512, false);
    heaps_.dsv.Initialize(device_.Get(), 512, false);
    CreateSwapChain();
    InitFrameResources();
    SetViewport();
}

// TODO: search for first avaible hardware adapter and look for best performance adapter (GPU)
// TODO: check for more features
void Graphics::InitDXGIAdapter() {
    u32 factoryFlags = 0;

#ifdef _DEBUG
    utl::EnableCpuLayer(factoryFlags);
    utl::EnableGpuLayer();
#endif
    CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&factory_)) >> utl::DxCheck;
    D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device_)) >> utl::DxCheck;

    if (SUCCEEDED(factory_->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing_, sizeof(u32))) &&
        allowTearing_) {
        presentInfo_ = DXGI_PRESENT_ALLOW_TEARING;
    }

#ifdef _DEBUG
    utl::QueueInfo(device_.Get(), TRUE);
#endif
}

void Graphics::CreateSwapChain() {
    ComPtr<IDXGISwapChain1> swapChain1;
    const DXGI_SWAP_CHAIN_DESC1 swapChainDesc{
            .Width = resolution_.width,
            .Height = resolution_.height,
            .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
            .Stereo = FALSE,
            .SampleDesc = {1, 0},
            .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
            .BufferCount = frameBufferCount,
            .Scaling = DXGI_SCALING_STRETCH,
            .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
            .AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
            .Flags = allowTearing_ ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0U
    };
    factory_->CreateSwapChainForHwnd(cmdManager_.GetQueue(), window_, &swapChainDesc, nullptr, nullptr, &swapChain1) >> utl::DxCheck;
    factory_->MakeWindowAssociation(window_, DXGI_MWA_NO_ALT_ENTER) >> utl::DxCheck; // Disable Alt + Enter for full screen window
    swapChain1.As(&swapChain_) >> utl::DxCheck;
}

void Graphics::InitFrameResources() {
    for(u32 i = 0; i < frameBufferCount; ++i) {
        D3D12_RENDER_TARGET_VIEW_DESC desc = {
                .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
                .ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D
        };
        backBuffers_[i].handle = heaps_.rtv.alloc();
        swapChain_->GetBuffer(i, IID_PPV_ARGS(&backBuffers_[i].resource)) >> utl::DxCheck;
        device_->CreateRenderTargetView(backBuffers_[i].resource.Get() ,&desc, backBuffers_[i].handle.cpu);
    }
}

void Graphics::SetViewport() {
    DXGI_SWAP_CHAIN_DESC desc {};

    swapChain_->GetDesc(&desc)  >> utl::DxCheck;
    viewport_.TopLeftX = 0.0f;
    viewport_.TopLeftY = 0.0f;
    viewport_.Height = (f32) desc.BufferDesc.Height;
    viewport_.Width = (f32) desc.BufferDesc.Width;
    viewport_.MaxDepth = 1.0f;
    viewport_.MinDepth = 0.0f;
}

void Graphics::LoadAssets() {
    cmdManager_.Reset();
    Vertex vertices[] = {
            { math::vec3(-1.0f, -1.0f, -1.0f), math::vec4(1.0f, 1.0f, 1.0f, 0.0f) },
            { math::vec3(-1.0f, +1.0f, -1.0f), math::vec4(0.0f, 0.0f, 0.0f, 0.0f) },
            { math::vec3(+1.0f, +1.0f, -1.0f), math::vec4(1.0f, 0.0f, 0.0f, 0.0f) },
            { math::vec3(+1.0f, -1.0f, -1.0f), math::vec4(0.0f, 1.0f, 0.0f, 0.0f) },
            { math::vec3(-1.0f, -1.0f, +1.0f), math::vec4(0.0f, 1.0f, 0.0f, 0.0f) },
            { math::vec3(-1.0f, +1.0f, +1.0f), math::vec4(0.0f, 1.0f, 0.0f, 0.0f) },
            { math::vec3(+1.0f, +1.0f, +1.0f), math::vec4(0.0f, 1.0f, 0.0f, 0.0f) },
            { math::vec3(+1.0f, -1.0f, +1.0f), math::vec4(0.0f, 1.0f, 0.0f, 0.0f) }
    };
    u16 indices[] = {
            // front face
            0, 1, 2,
            0, 2, 3,
            // back face
            4, 6, 5,
            4, 7, 6,
            // left face
            4, 5, 1,
            4, 1, 0,
            // right face
            3, 2, 6,
            3, 6, 7,
            // top face
            1, 5, 6,
            1, 6, 2,

            // bottom face
            4, 0, 3,
            4, 3, 7
    };

    BufferInfo vertexBufferInfo = {
            .device = device_.Get(),
            .cmdList = cmdManager_.List(),
            .data = &vertices,
            .byteSize = sizeof(Vertex) * 8,
            .byteStride = sizeof(Vertex)
    };

    BufferInfo indexBufferInfo = {
            .device = device_.Get(),
            .cmdList = cmdManager_.List(),
            .data = &vertices,
            .byteSize = sizeof(u16) * 36,
            .format = DXGI_FORMAT_R16_UINT
    };

    vertexBuffer_.Upload(vertexBufferInfo);
    indexBuffer_.Upload(indexBufferInfo);

    cmdManager_.List()->Close();
    cmdManager_.Execute();
    cmdManager_.WaitForGPU();

}

void Graphics::Update(render::Camera &camera) {}

// TODO: Functions to simplify barrier creation
void Graphics::PrepareRender() {

    auto &currentBuffer = backBuffers_[Commands::FrameIndex()];
    ID3D12GraphicsCommandList* commandList = cmdManager_.List();

    cmdManager_.Reset(); //Resets commands list and current frame allocator
    CleanDeferredResources(heaps_); // Clean deferreds resources



    auto targetBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
            currentBuffer.resource.Get(),
            D3D12_RESOURCE_STATE_PRESENT,
            D3D12_RESOURCE_STATE_RENDER_TARGET );
    commandList->ResourceBarrier(1, &targetBarrier);

    const f32 clearColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    commandList->ClearRenderTargetView(currentBuffer.handle.cpu, clearColor, 0, nullptr);

    auto presentBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
            currentBuffer.resource.Get(),
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_PRESENT
    );
    commandList->ResourceBarrier(1, &presentBarrier);
    commandList->Close() >> utl::DxCheck;
}

void Graphics::Draw() {
    cmdManager_.Execute();
    swapChain_->Present(0, presentInfo_) >> utl::DxCheck;
    cmdManager_.MoveToNextFrame();
}

void Graphics::Terminate() {
#ifdef _DEBUG
    utl::QueueInfo(device_.Get(), FALSE);
    utl::SetReporter(device_.Get());
#endif
    cmdManager_.Flush();
    heaps_.Release();
    vertexBuffer_.Release();
    indexBuffer_.Release();
    CleanDeferredResources(heaps_);
}


}
