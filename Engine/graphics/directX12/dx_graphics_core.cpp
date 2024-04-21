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
    heaps_.dsv.Initialize(device_.Get(), 1U, false);
    renderElements_.reserve(4092U);
    dsHandle_ = heaps_.dsv.alloc();
    CreateSwapChain();
    InitFrameResources();
    InitDsBuffer();
    InitConstantBuffers();
    SetViewport();
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
    factory_->CreateSwapChainForHwnd(cmdManager_.GetQueue(), window_.handle, &swapChainDesc, nullptr, nullptr, &swapChain1) >> utl::DxCheck;
    factory_->MakeWindowAssociation(window_.handle, DXGI_MWA_NO_ALT_ENTER) >> utl::DxCheck; // Disable Alt + Enter for full screen window
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
    renderLayers_.BuildRoots(device_.Get());
    renderLayers_.BuildPSOs(device_.Get());

    cmdManager_.Reset(nullptr);

    std::vector<core::Transform> &transforms = core::scene.Transforms();
    std::vector<core::Geometry> &geometries = core::scene.Geometries();

    for(u32 i = 0; i < core::scene.NumEntities(); ++i) {
        if (geometries[i].RenderInfo() == UINT_MAX) {
            BufferInitInfo vertexBufferInfo = {
                    .device = device_.Get(),
                    .cmdList = cmdManager_.List(),
                    .data = geometries[i].GetVerticesStart(),
                    .count = geometries[i].VertexCount()
            };

            BufferInitInfo indexBufferInfo = {
                    .device = device_.Get(),
                    .cmdList = cmdManager_.List(),
                    .data = geometries[i].GetIndicesStart(),
                    .count = geometries[i].IndexCount(),
                    .format = DXGI_FORMAT_R16_UINT
            };

            renderElements_.emplace_back(vertexBufferInfo, indexBufferInfo);
            geometries[i].SetRenderInfo(i);

            for (auto &subMesh: geometries[i].SubMeshes()) {
                subMesh.renderInfo = i;
                subMesh.constantIndex = i;
                renderLayers_.AddMesh(subMesh);
            }
        } else {
            for (auto &mesh : geometries[i].SubMeshes()) {
                mesh.renderInfo = geometries[i].RenderInfo();
                mesh.constantIndex = i;
                renderLayers_.AddMesh(mesh);
            }
        }

    }
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

    AlignedConstant<ObjConstant, 1> objConstant;
    for (u32 i = 0; i < core::scene.NumEntities(); ++i) {
        if (transforms[i].IsDirty() > 0) {
            objConstant.data.worldViewProj = transforms[i].World() * passConstant.data.viewProj;
            transforms[i].UpdateDirty();
            currFrameRes.constantBuffer.CopyData(i, &objConstant);
        }
    }
}

void Graphics::PrepareRender() {

    auto &currFrameRes = frameResources_[Commands::FrameIndex()];
    ID3D12GraphicsCommandList* commandList = cmdManager_.List();

    cmdManager_.Reset(renderLayers_[render::shader::opaque].pso.Get()); //Resets commands list and current frame allocator
    CleanDeferredResources(heaps_); // Clean deferreds resources

    commandList->RSSetViewports(1, &viewport_);
    commandList->RSSetScissorRects(1, &scissorRect_);

    auto targetBarrier = CD3DX12_RESOURCE_BARRIER::Transition( currFrameRes.backBuffer.Get(), D3D12_RESOURCE_STATE_PRESENT,
            D3D12_RESOURCE_STATE_RENDER_TARGET );
    commandList->ResourceBarrier(1, &targetBarrier);

    const f32 clearColor[] = { config::clearColor.x, config::clearColor.y, config::clearColor.z, config::clearColor.w };
    commandList->ClearRenderTargetView(currFrameRes.backBufferHandle.cpu, clearColor, 0, nullptr);
    commandList->ClearDepthStencilView(dsHandle_.cpu, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

    commandList->OMSetRenderTargets(1, &currFrameRes.backBufferHandle.cpu, TRUE, &dsHandle_.cpu);

    commandList->SetGraphicsRootSignature(renderLayers_[render::shader::opaque].rootSignature.Get());
    commandList->SetGraphicsRootConstantBufferView(1, currFrameRes.passBuffer.GpuStart());

    renderLayers_.DrawLayer(commandList, currFrameRes, renderElements_, render::shader::opaque);

    for (u32 i = 1; i < render::shader::count; ++i) {
        renderLayers_[i].Set(commandList);
        renderLayers_.DrawLayer(commandList, currFrameRes, renderElements_, i);
    }

    auto presentBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
            currFrameRes.backBuffer.Get(),
            D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT );
    commandList->ResourceBarrier(1, &presentBarrier);
    commandList->Close() >> utl::DxCheck;
}


void Graphics::Draw() {
    cmdManager_.Execute();
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
}