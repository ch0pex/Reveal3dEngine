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
    renderElements_.reserve(4092U);
    dsHandle_ = heaps_.dsv.alloc();
    surface_.Init(cmdManager_, factory_.Get());
    InitFrameResources();
    InitDsBuffer();
    InitConstantBuffers();
    surface_.UpdateViewport();
    renderLayers_.BuildRoots(device_.Get());
    renderLayers_.BuildPSOs(device_.Get());

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
        frameResource.constantBuffer.Init(device_.Get(), 65536U); //TODO: hardcoded capacity 256 maximum?
        frameResource.matBuffer.Init(device_.Get(), 65536U); //TODO: hardcoded capacity 256 maximum?
        frameResource.passBuffer.Init(device_.Get(),  1U); //TODO: hardcoded capacity
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
}


void Dx12::LoadAssets() {
    cmdManager_.Reset(nullptr);

    core::TransformPool& transforms = core::scene.ComponentPool<core::Transform>();
    // auto& geometries = core::scene.ComponentPool<Geometry>();
    core::GeometryPool& geometries = core::scene.ComponentPool<core::Geometry>();

    core::Geometry geometry = geometries.PopNewGeometry();

    while(geometry.IsAlive()) {
        id_t idx = id::index(geometry.Id());
        core::Transform transform = transforms.At(idx);
        CreateRenderElement(idx);
        Constant<PerObjectData> objConstant;
        Constant<Material> matConstant;
        for (auto & frameResource : frameResources_) {
            objConstant.data.worldViewProj = transform.World();
            matConstant.data.baseColor = geometry.Material().baseColor;
            frameResource.constantBuffer.CopyData(idx, &objConstant, 1);
            frameResource.matBuffer.CopyData(idx, &matConstant, 1);
        }

        geometry = geometries.PopNewGeometry();
    }

    cmdManager_.List()->Close();
    cmdManager_.Execute();
    cmdManager_.WaitForGPU();

}

void Dx12::LoadAsset(u32 id) {
    cmdManager_.Reset(nullptr);
    CreateRenderElement(id);
    cmdManager_.List()->Close();
    cmdManager_.Execute();
    cmdManager_.WaitForGPU();
}

void Dx12::Update(render::Camera &camera) {
    auto &currFrameRes = frameResources_.at(Commands::FrameIndex());
    std::set<id_t>& dirtyTransforms = core::scene.ComponentPool<core::Transform>().DirtyElements();
    std::set<id_t>& dirtyMats = core::scene.ComponentPool<core::Geometry>().DirtyElements();
    core::GeometryPool& geometries = core::scene.ComponentPool<core::Geometry>();
    core::Geometry new_geo = geometries.PopNewGeometry();
    core::Geometry removed_geo = geometries.PopRemovedGeometry();


    Constant<GlobalShaderData> passConstant;
    Constant<PerObjectData> objConstant;
    Constant<Material> matConstant;

    // Update pass constants
    passConstant.data.viewProj = math::Transpose(camera.GetViewProjectionMatrix());
    currFrameRes.passBuffer.CopyData(0, &passConstant);

    // Update object constants
    for (auto id : dirtyTransforms) {
        core::Transform trans = core::scene.ComponentPool<core::Transform>().At(id);
        objConstant.data.worldViewProj = trans.World();
        trans.UnDirty();
        currFrameRes.constantBuffer.CopyData(id::index(id), &objConstant);
    }

    // Update material constants
    for (auto id : dirtyMats) {
        core::Geometry geo = core::scene.ComponentPool<core::Geometry>().At(id);
        matConstant.data.baseColor = geo.Material().baseColor;
        geo.UnDirty();
        currFrameRes.matBuffer.CopyData(id::index(id), &matConstant);
    }

    // Add new meshes
    while(new_geo.IsAlive()) {
        LoadAsset(new_geo.Id());
        new_geo = geometries.PopNewGeometry();
    }

    while(removed_geo.IsAlive()) {
        RemoveAsset(removed_geo.Id());
        removed_geo = geometries.PopRemovedGeometry();
    }

}

void Dx12::RenderSurface() {

    auto &currFrameRes = frameResources_[Commands::FrameIndex()];
    ID3D12GraphicsCommandList* commandList = cmdManager_.List();
    cmdManager_.Reset(renderLayers_[render::Shader::opaque].pso.Get()); //Resets commands list and current frame allocator

//    cmdManager_.Reset(renderLayers_[render::Shader::opaque].pso.Get()); //Resets commands list and current frame allocator
    CleanDeferredResources(heaps_); // Clean deferreds resources

    surface_.SetViewport(commandList);

    auto targetBarrier = CD3DX12_RESOURCE_BARRIER::Transition(currFrameRes.backBuffer.Get(), D3D12_RESOURCE_STATE_PRESENT,
            D3D12_RESOURCE_STATE_RENDER_TARGET);
    commandList->ResourceBarrier(1, &targetBarrier);

    const f32 clearColor[] = { config::clearColor.x, config::clearColor.y, config::clearColor.z, config::clearColor.w };
    commandList->ClearRenderTargetView(currFrameRes.backBufferHandle.cpu, clearColor, 0, nullptr);
    commandList->ClearDepthStencilView(dsHandle_.cpu, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

    commandList->OMSetRenderTargets(1, &currFrameRes.backBufferHandle.cpu, TRUE, &dsHandle_.cpu);
    commandList->SetGraphicsRootSignature(renderLayers_[render::Shader::opaque].rootSignature.Get());
    commandList->SetGraphicsRootConstantBufferView(2, currFrameRes.passBuffer.GpuStart());
    ID3D12DescriptorHeap* srvDesc = heaps_.srv.Get();
    commandList->SetDescriptorHeaps(1, &srvDesc);

    renderLayers_.DrawLayer(commandList, currFrameRes, renderElements_, render::Shader::opaque);

    for (u32 i = 1; i < render::Shader::count - 1; ++i) {
        renderLayers_[i].Set(commandList);
        renderLayers_.DrawLayer(commandList, currFrameRes, renderElements_, i);
    }

    renderLayers_[render::Shader::grid].Set(commandList);
    renderLayers_.DrawEffectLayer(commandList, render::Shader::grid);

    auto presentBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
            currFrameRes.backBuffer.Get(),
            D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT );

#ifdef IMGUI
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

    cmdManager_.WaitForGPU();

    cmdManager_.Reset(nullptr);

    for(auto &frameResource : frameResources_){
        frameResource.backBuffer.Reset();
    }

    depthStencilBuffer_.Reset();
    InitDsBuffer();

    surface_.Resolution();

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

    for (auto &elem : renderElements_) {
        elem.vertexBuffer.Release();
        elem.indexBuffer.Release();
    }

    for (auto& frameResource : frameResources_) {
        frameResource.constantBuffer.Release();
        frameResource.passBuffer.Release();
        frameResource.matBuffer.Release();
    }
    CleanDeferredResources(heaps_);
}

void Dx12::CreateRenderElement(u32 index) {
    auto geometry = core::scene.GetEntity(index).Component<core::Geometry>();
    if (geometry.RenderInfo() == UINT_MAX) {
        BufferInitInfo vertexBufferInfo = {
                .device = device_.Get(),
                .cmdList = cmdManager_.List(),
                .data = geometry.Vertices().data(),
                .count = geometry.VertexCount()
        };

        BufferInitInfo indexBufferInfo = {
                .device = device_.Get(),
                .cmdList = cmdManager_.List(),
                .data = geometry.Indices().data(),
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

void Dx12::RemoveAsset(id_t id) {
    renderElements_.erase(renderElements_.begin() + id::index(id));
}


}