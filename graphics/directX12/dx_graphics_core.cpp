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
#include "core/components.hpp"
namespace reveal3d::graphics {

using namespace render;
using namespace dx12;

Dx12::Dx12(window::Resolution *res) : surface_(*res)
{

}

void Dx12::LoadPipeline() {
    InitDXGIAdapter();
    cmdManager_.Init(device_.Get());
    heaps_.rtv.Initialize(device_.Get(), dx12::frameBufferCount, false);
    heaps_.srv.Initialize(device_.Get(), 1U, true);
    heaps_.dsv.Initialize(device_.Get(), 1U, false);
    dsHandle_ = heaps_.dsv.alloc();
    surface_.Init(cmdManager_, factory_.Get());
    InitFrameResources();
    InitDsBuffer();
    InitConstantBuffers();
    surface_.UpdateViewport();
    gPass_.Init(device_.Get());

}

// TODO: search for first avaible hardware adapter and look for best performance adapter (GPU)
// TODO: check for more features
void Dx12::InitDXGIAdapter() {
    u32 factoryFlags = 0;

#ifdef _DEBUG
    utl::EnableCpuLayer(factoryFlags);
    utl::LogAdapters();
    utl::EnableGpuLayer();
#endif
    ComPtr<IDXGIAdapter1> hardwareAdapter;
    CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&factory_)) >> utl::DxCheck;
    utl::GetHardwareAdapter(factory_.Get(), &hardwareAdapter);
    D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device_)) >> utl::DxCheck;

    surface_.AllowTearing(factory_.Get());

#ifdef _DEBUG
    utl::QueueInfo(device_.Get(), TRUE);
#endif
}

void Dx12::InitFrameResources() {
    for(u32 i = 0; i < frameBufferCount; ++i) {
        D3D12_RENDER_TARGET_VIEW_DESC desc = {
                .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
                .ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D
        };
        frameResources_[i].backBufferHandle = heaps_.rtv.alloc();
        surface_.GetBuffer(i, frameResources_[i].backBuffer);
        device_->CreateRenderTargetView(frameResources_[i].backBuffer.Get() ,&desc, frameResources_[i].backBufferHandle.cpu);
        std::wstring name = L"BackBuffer " + std::to_wstring(i);
        frameResources_[i].backBuffer->SetName(name.c_str()) >> utl::DxCheck;
    }
}

void Dx12::InitConstantBuffers() {
    for(auto& frameResource : frameResources_) {
        frameResource.constantBuffer.Init(device_.Get(), 65536U);
        frameResource.matBuffer.Init(device_.Get(), 65536U);
        frameResource.passBuffer.Init(device_.Get(),  1U);
    }
}

void Dx12::InitDsBuffer() {
    const D3D12_RESOURCE_DESC depthStencilDesc = {
            .Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D,
            .Alignment = 0,
            .Width = surface_.Resolution().width,
            .Height = surface_.Resolution().height,
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

    for(auto& frameResource : frameResources_) {
        frameResource.depthBufferHandle = dsHandle_;
    }
}


void Dx12::LoadAssets() {
    cmdManager_.Reset(nullptr);
    using namespace core;

    Pool<Transform>& transforms = core::scene.ComponentPool<core::Transform>();
    // auto& geometries = core::scene.ComponentPool<Geometry>();
    Pool<Geometry>& geometries = core::scene.ComponentPool<core::Geometry>();

    auto entity = core::Entity(geometries.PopNew());
    auto geometry = entity.Component<core::Geometry>();

    while(geometry.IsAlive()) {
        id_t idx = id::index(geometry.Id());
        auto transform = entity.Component<core::Transform>();
        gPass_.AddRenderElement(entity, cmdManager_, device_.Get());
        Constant<PerObjectData> objConstant;
        Constant<Material> matConstant;
        for (auto & frameResource : frameResources_) {
            objConstant.data.worldViewProj = transform.World();
            matConstant.data.baseColor = geometry.Material().baseColor;
            frameResource.constantBuffer.CopyData(idx, &objConstant, 1);
            frameResource.matBuffer.CopyData(idx, &matConstant, 1);
        }

        entity = core::Entity(geometries.PopNew());
        geometry = entity.Component<core::Geometry>();
    }

    cmdManager_.List()->Close();
    cmdManager_.Execute();
    cmdManager_.WaitForGPU();

}

void Dx12::LoadAsset(core::Entity entity) {
    cmdManager_.Reset(nullptr);
    gPass_.AddRenderElement(entity, cmdManager_, device_.Get());
    cmdManager_.List()->Close();
    cmdManager_.Execute();
    cmdManager_.WaitForGPU();
}

void Dx12::Update(render::Camera &camera) {
    auto &currFrameRes = frameResources_.at(Commands::FrameIndex());
    std::set<id_t>& dirtyTransforms = core::scene.ComponentPool<core::Transform>().DirtyElements();
    std::set<id_t>& dirtyMats = core::scene.ComponentPool<core::Geometry>().DirtyElements();
    core::Pool<core::Geometry>& geometries = core::scene.ComponentPool<core::Geometry>();
    auto entityWithNewGeo = core::Entity(geometries.PopNew());
    auto entityWithRemovedGeo = geometries.PopRemoved();

    Constant<GlobalShaderData> passConstant;
    Constant<PerObjectData> objConstant;
    Constant<Material> matConstant;

    // Update pass constants
    passConstant.data.viewProj = math::Transpose(camera.GetViewProjectionMatrix());
    currFrameRes.passBuffer.CopyData(0, &passConstant);

    // Update object constants
    for (auto id : dirtyTransforms) {
        core::Transform trans { id } ;
        objConstant.data.worldViewProj = trans.World();
        trans.UnDirty();
        currFrameRes.constantBuffer.CopyData(id::index(id), &objConstant);
    }

    // Update material constants
    for (auto id : dirtyMats) {
        core::Geometry geo { id } ;
        matConstant.data.baseColor = geo.Material().baseColor;
        geo.UnDirty();
        currFrameRes.matBuffer.CopyData(id::index(geo.Id()), &matConstant);
    }

    // Add new meshes
    while(entityWithNewGeo.IsAlive()) {
        LoadAsset(entityWithNewGeo);
        entityWithNewGeo = core::Entity(geometries.PopNew());
    }

    while(entityWithRemovedGeo != id::invalid) {
        gPass_.RemoveRenderElement(entityWithRemovedGeo);
        entityWithRemovedGeo = geometries.PopRemoved();
    }

}

void Dx12::RenderSurface() {
    auto &currFrameRes = frameResources_[Commands::FrameIndex()];
    ID3D12GraphicsCommandList* commandList = cmdManager_.List();

    cmdManager_.Reset(); //Resets commands list and current frame allocator

    CleanDeferredResources(heaps_); // Clean deferreds resources

    surface_.SetViewport(commandList);

    auto targetBarrier = CD3DX12_RESOURCE_BARRIER::Transition(currFrameRes.backBuffer.Get(), D3D12_RESOURCE_STATE_PRESENT,
            D3D12_RESOURCE_STATE_RENDER_TARGET);
    commandList->ResourceBarrier(1, &targetBarrier);

    gPass_.Render(commandList, currFrameRes);

    auto presentBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
            currFrameRes.backBuffer.Get(),
            D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT );


#ifdef IMGUI
    ID3D12DescriptorHeap* srvDesc = heaps_.srv.Get();
    commandList->SetDescriptorHeaps(1, &srvDesc);
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
#endif

    commandList->ResourceBarrier(1, &presentBarrier);
    commandList->Close() >> utl::DxCheck;
    cmdManager_.Execute();

#ifdef IMGUI
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault(nullptr, (void*)cmdManager_.List());
#endif
    surface_.Present();
    cmdManager_.MoveToNextFrame();
}

void Dx12::Resize(const window::Resolution &res) {
    if (res.aspectRatio == surface_.Resolution().aspectRatio) {
        return;
    }

    if (res.width == 0 and res.height == 0) {
        return;
    }

    cmdManager_.WaitForGPU();

    cmdManager_.Reset(nullptr);

    for(auto &frameResource : frameResources_){
        frameResource.backBuffer.Reset();
    }

    depthStencilBuffer_.Reset();
    surface_.Resize(res);
    InitDsBuffer();
    cmdManager_.ResetFences();

    for (u32 i = 0; i < frameBufferCount; ++i) {
        D3D12_RENDER_TARGET_VIEW_DESC desc = {
                .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
                .ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D
        };
        surface_.GetBuffer(i, frameResources_[i].backBuffer);
        device_->CreateRenderTargetView(frameResources_[i].backBuffer.Get() ,&desc, frameResources_[i].backBufferHandle.cpu);
    }

    cmdManager_.List()->Close();
    cmdManager_.Execute();
    cmdManager_.WaitForGPU();

    surface_.UpdateViewport();
}

void Dx12::Terminate() {
#ifdef _DEBUG
    utl::QueueInfo(device_.Get(), FALSE);
    utl::SetReporter(device_.Get());
#endif
    cmdManager_.Flush();
    heaps_.Release();
    gPass_.Terminate();

    for (auto& frameResource : frameResources_) {
        frameResource.constantBuffer.Release();
        frameResource.passBuffer.Release();
        frameResource.matBuffer.Release();
    }
    CleanDeferredResources(heaps_);
}

void Dx12::RemoveAsset(id_t id) {
//    gPass_.RemoveRenderElement();
}


}