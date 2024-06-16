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

#ifdef IMGUI
#include "IMGUI/imgui.h"
#include "IMGUI/backends/imgui_impl_dx12.h"
//#include "IMGUI/imgui_internal.h"
#endif

#include "dx_graphics_core.hpp"
#include "config/config.hpp"
namespace reveal3d::graphics::dx {

using namespace render;

Graphics::Graphics(window::Resolution *res) :
    resolution_(res), presentInfo_(0),
    swapChainFlags_(DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH)
{

}

void Graphics::LoadPipeline() {
    InitDXGIAdapter();
    cmdManager_.Init(device_.Get());
    heaps_.rtv.Initialize(device_.Get(), frameBufferCount, false);
    heaps_.srv.Initialize(device_.Get(), 1U, true);
    heaps_.dsv.Initialize(device_.Get(), 1U, false);
    renderElements_.reserve(4092U);
    dsHandle_ = heaps_.dsv.alloc();
    CreateSwapChain();
    InitFrameResources();
    InitDsBuffer();
    InitConstantBuffers();
    SetViewport();
    renderLayers_.BuildRoots(device_.Get());
    renderLayers_.BuildPSOs(device_.Get());

}

// TODO: search for first avaible hardware adapter and look for best performance adapter (GPU)
// TODO: check for more features
void Graphics::InitDXGIAdapter() {
    u32 factoryFlags = 0;
    BOOL allowTearing = FALSE;

#ifdef _DEBUG
    utl::EnableCpuLayer(factoryFlags);
    utl::LogAdapters();
    utl::EnableGpuLayer();
#endif
    ComPtr<IDXGIAdapter1> hardwareAdapter;
    CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&factory_)) >> utl::DxCheck;
    GetHardwareAdapter(factory_.Get(), &hardwareAdapter);
    D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device_)) >> utl::DxCheck;

    if (SUCCEEDED(factory_->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(u32))) &&
        allowTearing) {
        presentInfo_ = DXGI_PRESENT_ALLOW_TEARING;
        swapChainFlags_ |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
    }

#ifdef _DEBUG
    utl::QueueInfo(device_.Get(), TRUE);
#endif
}

void Graphics::CreateSwapChain() {
    ComPtr<IDXGISwapChain1> swapChain1;

    const DXGI_SWAP_CHAIN_DESC1 swapChainDesc{
            .Width = resolution_->width,
            .Height = resolution_->height,
            .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
            .Stereo = FALSE,
            .SampleDesc = {1, 0},
            .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
            .BufferCount = frameBufferCount,
            .Scaling = DXGI_SCALING_STRETCH,
            .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
            .AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
            .Flags = swapChainFlags_
    };
    factory_->CreateSwapChainForHwnd(cmdManager_.GetQueue(), window_.hwnd, &swapChainDesc, nullptr, nullptr, &swapChain1) >> utl::DxCheck;
    factory_->MakeWindowAssociation(window_.hwnd, DXGI_MWA_NO_ALT_ENTER) >> utl::DxCheck; // Disable Alt + Enter for full screen window
    swapChain1.As(&swapChain_) >> utl::DxCheck;
}

void Graphics::InitFrameResources() {
    for(u32 i = 0; i < frameBufferCount; ++i) {
        D3D12_RENDER_TARGET_VIEW_DESC desc = {
                .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
                .ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D
        };
        frameResources_[i].backBufferHandle = heaps_.rtv.alloc();
        swapChain_->GetBuffer(i, IID_PPV_ARGS(&frameResources_[i].backBuffer)) >> utl::DxCheck;
        device_->CreateRenderTargetView(frameResources_[i].backBuffer.Get() ,&desc, frameResources_[i].backBufferHandle.cpu);
        std::wstring name = L"BackBuffer " + std::to_wstring(i);
        frameResources_[i].backBuffer->SetName(name.c_str()) >> utl::DxCheck;
    }
}

void Graphics::InitConstantBuffers() {
    for(auto& frameResource : frameResources_) {
        frameResource.constantBuffer.Init(device_.Get(), 65536U); //TODO: hardcoded capacity 256 maximum?
        frameResource.passBuffer.Init(device_.Get(),  1U); //TODO: hardcoded capacity
//        frameResource.passHandle = frameResource.passBuffer.CreateView(device_.Get(), heaps_.cbv);
    }
}

void Graphics::InitDsBuffer() {
    const D3D12_RESOURCE_DESC depthStencilDesc = {
            .Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D,
            .Alignment = 0,
            .Width = resolution_->width,
            .Height = resolution_->height,
            .DepthOrArraySize = 1,
            .MipLevels = 1,
            .Format = DXGI_FORMAT_R24G8_TYPELESS,
            .SampleDesc = {
                    .Count =  1,
                    .Quality =  0,
            },
            .Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN,
            .Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL,
    };

    const D3D12_CLEAR_VALUE optClear = {
            .Format = DXGI_FORMAT_D24_UNORM_S8_UINT,
            .DepthStencil = {
                .Depth = 1.0f,
                .Stencil = 0,
            }
    };

    auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    device_->CreateCommittedResource(
            &heapProp,
            D3D12_HEAP_FLAG_NONE,
            &depthStencilDesc,
            D3D12_RESOURCE_STATE_DEPTH_WRITE,
            &optClear,
            IID_PPV_ARGS(depthStencilBuffer_.GetAddressOf())) >> utl::DxCheck;

    const D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {
            .Format = DXGI_FORMAT_D24_UNORM_S8_UINT,
            .ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D,
            .Flags = D3D12_DSV_FLAG_NONE,
            .Texture2D = {
                    .MipSlice = 0
            }
    };

    depthStencilBuffer_->SetName(L"Depth Buffer");
    device_->CreateDepthStencilView(depthStencilBuffer_.Get(), &dsvDesc, dsHandle_.cpu);
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
    scissorRect_ = { 0, 0, (i32) desc.BufferDesc.Width, (i32) desc.BufferDesc.Height };
}

void Graphics::LoadAssets() {
    cmdManager_.Reset(nullptr);

    std::vector<core::Transform> &transforms = core::scene.Transforms();
    std::vector<core::Geometry> &geometries = core::scene.Geometries();

    for(u32 i = 0; i < core::scene.NumEntities(); ++i) {
        CreateRenderElement(i);
        AlignedConstant<ObjConstant, 1> objConstant;
        for (u32 j = 0; j < frameBufferCount; ++j) {
            objConstant.data.worldViewProj = transforms[i].World();
            objConstant.data.flatColor = geometries[i].Color();
            frameResources_[j].constantBuffer.CopyData(i, &objConstant, 1);
        }
    }
    cmdManager_.List()->Close();
    cmdManager_.Execute();
    cmdManager_.WaitForGPU();

}

void Graphics::LoadAsset(u32 id) {
    cmdManager_.Reset(nullptr);
    CreateRenderElement(id);
    cmdManager_.List()->Close();
    cmdManager_.Execute();
    cmdManager_.WaitForGPU();
}

void Graphics::Update(render::Camera &camera) {
    auto &currFrameRes = frameResources_[Commands::FrameIndex()];
    AlignedConstant<PassConstant, 2> passConstant;

    passConstant.data.viewProj = math::Transpose(camera.GetViewProjectionMatrix());
    currFrameRes.passBuffer.CopyData(0, &passConstant);

    auto &transforms = core::scene.Transforms();
    auto &geometries = core::scene.Geometries();

    AlignedConstant<ObjConstant, 1> objConstant;
    for (u32 i = 0; i < core::scene.NumEntities(); ++i) {
        if (!geometries[i].OnGpu())
            LoadAsset(i);
        if (transforms[i].IsDirty() > 0) {
            objConstant.data.flatColor = geometries[i].Color();
            objConstant.data.worldViewProj = transforms[i].World();
            transforms[i].UpdateDirty();
            currFrameRes.constantBuffer.CopyData(i, &objConstant);
        }
    }
}

void Graphics::PrepareRender() {

    auto &currFrameRes = frameResources_[Commands::FrameIndex()];
    ID3D12GraphicsCommandList* commandList = cmdManager_.List();
    cmdManager_.Reset(renderLayers_[render::Shader::opaque].pso.Get()); //Resets commands list and current frame allocator

//    cmdManager_.Reset(renderLayers_[render::Shader::opaque].pso.Get()); //Resets commands list and current frame allocator
    CleanDeferredResources(heaps_); // Clean deferreds resources

    commandList->RSSetViewports(1, &viewport_);
    commandList->RSSetScissorRects(1, &scissorRect_);

    auto targetBarrier = CD3DX12_RESOURCE_BARRIER::Transition(currFrameRes.backBuffer.Get(), D3D12_RESOURCE_STATE_PRESENT,
            D3D12_RESOURCE_STATE_RENDER_TARGET);
    commandList->ResourceBarrier(1, &targetBarrier);

    const f32 clearColor[] = { config::clearColor.x, config::clearColor.y, config::clearColor.z, config::clearColor.w };
    commandList->ClearRenderTargetView(currFrameRes.backBufferHandle.cpu, clearColor, 0, nullptr);
    commandList->ClearDepthStencilView(dsHandle_.cpu, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

    commandList->OMSetRenderTargets(1, &currFrameRes.backBufferHandle.cpu, TRUE, &dsHandle_.cpu);
    commandList->SetGraphicsRootSignature(renderLayers_[render::Shader::opaque].rootSignature.Get());
    commandList->SetGraphicsRootConstantBufferView(1, currFrameRes.passBuffer.GpuStart());
    ID3D12DescriptorHeap* srvDesc = heaps_.srv.Get();
    commandList->SetDescriptorHeaps(1, &srvDesc);

    renderLayers_.DrawLayer(commandList, currFrameRes, renderElements_, render::Shader::opaque);

    for (u32 i = 0; i < render::Shader::count; ++i) {
        renderLayers_[i].Set(commandList);
        renderLayers_.DrawLayer(commandList, currFrameRes, renderElements_, i);
    }

    auto presentBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
            currFrameRes.backBuffer.Get(),
            D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT );

#ifdef IMGUI
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
#endif

    commandList->ResourceBarrier(1, &presentBarrier);
    commandList->Close() >> utl::DxCheck;
}


void Graphics::Draw() {
    cmdManager_.Execute();
#ifdef IMGUI
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault(nullptr, (void*)cmdManager_.List());
#endif
    swapChain_->Present(0, presentInfo_) >> utl::DxCheck;
    cmdManager_.MoveToNextFrame();
}

void Graphics::Resize(const window::Resolution &res) {
    if (res.aspectRatio == resolution_->aspectRatio) {
        return;
    }

    *resolution_ = res;
    cmdManager_.WaitForGPU();

    cmdManager_.Reset(nullptr);

    for(auto &frameResource : frameResources_){
        frameResource.backBuffer.Reset();
    }

    depthStencilBuffer_.Reset();
    InitDsBuffer();

    swapChain_->ResizeBuffers(frameBufferCount, resolution_->width, resolution_->height, DXGI_FORMAT_R8G8B8A8_UNORM,
            swapChainFlags_) >> utl::DxCheck;

    cmdManager_.ResetFences();

    for (u32 i = 0; i < frameBufferCount; ++i) {
        D3D12_RENDER_TARGET_VIEW_DESC desc = {
                .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
                .ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D
        };
        swapChain_->GetBuffer(i, IID_PPV_ARGS(&frameResources_[i].backBuffer));
        device_->CreateRenderTargetView(frameResources_[i].backBuffer.Get() ,&desc, frameResources_[i].backBufferHandle.cpu);
    }

    cmdManager_.List()->Close();
    cmdManager_.Execute();
    cmdManager_.WaitForGPU();

    SetViewport();
}

void Graphics::Terminate() {
#ifdef _DEBUG
    utl::QueueInfo(device_.Get(), FALSE);
    utl::SetReporter(device_.Get());
#endif
    cmdManager_.Flush();
    heaps_.Release();

    for (auto &elem : renderElements_) {
        elem.vertexBuffer.Release();
        elem.indexBuffer.Release();
    }

    for (auto& frameResource : frameResources_) {
        frameResource.constantBuffer.Release();
        frameResource.passBuffer.Release();
    }
    CleanDeferredResources(heaps_);
}

void Graphics::GetHardwareAdapter(IDXGIFactory1 *pFactory, IDXGIAdapter1 **ppAdapter)
{
    *ppAdapter = nullptr;

    ComPtr<IDXGIAdapter1> adapter;
    ComPtr<IDXGIFactory6> factory6;
    if (SUCCEEDED(pFactory->QueryInterface(IID_PPV_ARGS(&factory6))))
    {
        for ( u32 adapterIndex = 0; SUCCEEDED(factory6->EnumAdapterByGpuPreference( adapterIndex, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter))); ++adapterIndex) {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                // Don't select the Basic Render Driver adapter.
                // If you want a software adapter, pass in "/warp" on the command line.
                continue;
            }

            // Check to see whether the adapter supports Direct3D 12, but don't create the
            // actual device yet.
            if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), nullptr)))
            {
                break;
            }
        }
    }

    if(adapter.Get() == nullptr)
    {
        for (UINT adapterIndex = 0; SUCCEEDED(pFactory->EnumAdapters1(adapterIndex, &adapter)); ++adapterIndex)
        {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                // Don't select the Basic Render Driver adapter.
                // If you want a software adapter, pass in "/warp" on the command line.
                continue;
            }

            // Check to see whether the adapter supports Direct3D 12, but don't create the
            // actual device yet.
            if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), nullptr)))
            {
                break;
            }
        }
    }

    *ppAdapter = adapter.Detach();
}

void Graphics::CreateRenderElement(u32 index) {
    core::Geometry &geometry = core::scene.GetGeometry(index);
    geometry.MarkAsStored();
    if (geometry.RenderInfo() == UINT_MAX) {
        BufferInitInfo vertexBufferInfo = {
                .device = device_.Get(),
                .cmdList = cmdManager_.List(),
                .data = geometry.GetVerticesStart(),
                .count = geometry.VertexCount()
        };

        BufferInitInfo indexBufferInfo = {
                .device = device_.Get(),
                .cmdList = cmdManager_.List(),
                .data = geometry.GetIndicesStart(),
                .count = geometry.IndexCount(),
                .format = DXGI_FORMAT_R32_UINT
        };

        renderElements_.emplace_back(vertexBufferInfo, indexBufferInfo);
        geometry.SetRenderInfo(renderElements_.size() - 1U);
        for (auto &subMesh: geometry.SubMeshes()) {
            subMesh.renderInfo = renderElements_.size() - 1U;
            subMesh.constantIndex = index;
            renderLayers_.AddMesh(subMesh);
        }
    }
    else {
        for (auto &subMesh: geometry.SubMeshes()) {
            subMesh.renderInfo = geometry.RenderInfo();
            subMesh.constantIndex = index;
            renderLayers_.AddMesh(subMesh);
        }
    }

}


}