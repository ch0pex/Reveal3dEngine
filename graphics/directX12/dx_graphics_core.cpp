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

void Dx12::loadPipeline() {
    initDxgiAdapter();
    cmd_manager_.init(device_.Get());
    heaps_.rtv.initialize(device_.Get(), dx12::frameBufferCount, false);
    heaps_.srv.initialize(device_.Get(), 1U, true);
    heaps_.dsv.initialize(device_.Get(), 1U, false);
    ds_handle_ = heaps_.dsv.alloc();
    surface_.init(cmd_manager_, factory_.Get());
    initFrameResources();
    initDsBuffer();
    initConstantBuffers();
    surface_.updateViewport();
    gpass_.init(device_.Get());

}

// TODO: search for first avaible hardware adapter and look for best performance adapter (GPU)
// TODO: check for more features
void Dx12::initDxgiAdapter() {

    u32 factory_flags = 0;
#ifdef _DEBUG
    utl::enable_cpu_layer(factory_flags);
    utl::log_adapters();
    utl::enable_gpu_layer();
#endif
    ComPtr<IDXGIAdapter1> hardware_adapter;
    CreateDXGIFactory2(factory_flags, IID_PPV_ARGS(&factory_)) >> utl::DxCheck;
    utl::get_hardware_adapter(factory_.Get(), &hardware_adapter);
    D3D12CreateDevice(hardware_adapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device_)) >> utl::DxCheck;

    surface_.allowTearing(factory_.Get());

#ifdef _DEBUG
    utl::queue_info(device_.Get(), TRUE);
#endif
}

void Dx12::initFrameResources() {
    for(u32 i = 0; i < frameBufferCount; ++i) {
        D3D12_RENDER_TARGET_VIEW_DESC desc = {
                .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
                .ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D
        };
        frame_resources_[i].back_buffer_handle = heaps_.rtv.alloc();
        surface_.getBuffer(i, frame_resources_[i].back_buffer);
        device_->CreateRenderTargetView(frame_resources_[i].back_buffer.Get() ,&desc,
                                        frame_resources_[i].back_buffer_handle.cpu);
        std::wstring name = L"BackBuffer " + std::to_wstring(i);
        frame_resources_[i].back_buffer->SetName(name.c_str()) >> utl::DxCheck;
    }
}

void Dx12::initConstantBuffers() {
    for(auto&frame_resource: frame_resources_) {
        frame_resource.constant_buffer.init(device_.Get(), 100'000);
        frame_resource.mat_buffer.init(device_.Get(), 100'000);
        frame_resource.pass_buffer.init(device_.Get(), 1U)   ;
    }
}

void Dx12::initDsBuffer() {
    const D3D12_RESOURCE_DESC depth_stencil_desc = {
            .Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D,
            .Alignment = 0,
            .Width = surface_.resolution().width,
            .Height = surface_.resolution().height,
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

    const D3D12_CLEAR_VALUE opt_clear = {
            .Format = DXGI_FORMAT_D24_UNORM_S8_UINT,
            .DepthStencil = {
                .Depth = 1.0f,
                .Stencil = 0,
            }
    };

    auto heap_prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    device_->CreateCommittedResource(
            &heap_prop,
            D3D12_HEAP_FLAG_NONE,
            &depth_stencil_desc,
            D3D12_RESOURCE_STATE_DEPTH_WRITE,
            &opt_clear,
            IID_PPV_ARGS(depth_stencil_buffer_.GetAddressOf())) >> utl::DxCheck;

    const D3D12_DEPTH_STENCIL_VIEW_DESC dsv_desc = {
            .Format = DXGI_FORMAT_D24_UNORM_S8_UINT,
            .ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D,
            .Flags = D3D12_DSV_FLAG_NONE,
            .Texture2D = {
                    .MipSlice = 0
            }
    };

    depth_stencil_buffer_->SetName(L"Depth Buffer");
    device_->CreateDepthStencilView(depth_stencil_buffer_.Get(), &dsv_desc, ds_handle_.cpu);

    for(auto& frame_resource: frame_resources_) {
        frame_resource.depth_buffer_handle = ds_handle_;
    }
}


void Dx12::loadAssets() {
    cmd_manager_.reset(nullptr);
    using namespace core;

    Pool<Transform>& transforms = core::scene.componentPool<core::Transform>();
    // auto& geometries = core::scene.componentPool<Geometry>();
    Pool<Geometry>& geometries = core::scene.componentPool<core::Geometry>();

    auto entity = core::Entity(geometries.popNew());
    auto geometry = entity.component<core::Geometry>();

    while(geometry.isAlive()) {
        id_t idx = id::index(geometry.id());
        auto transform = entity.component<core::Transform>();
        gpass_.addRenderElement(entity, cmd_manager_, device_.Get());
        Constant<PerObjectData> obj_constant;
        Constant<Material> mat_constant;
        for (auto &frame_resource: frame_resources_) {
            obj_constant.data.world_view_proj = transform.world();
            mat_constant.data.base_color = geometry.material().base_color;
            frame_resource.constant_buffer.copyData(idx, &obj_constant, 1);
            frame_resource.mat_buffer.copyData(idx, &mat_constant, 1);
        }

        entity = core::Entity(geometries.popNew());
        geometry = entity.component<core::Geometry>();
    }

    cmd_manager_.list()->Close();
    cmd_manager_.execute();
    cmd_manager_.waitForGpu();

}

void Dx12::loadAsset(core::Entity id) {
    cmd_manager_.reset(nullptr);
    gpass_.addRenderElement(id, cmd_manager_, device_.Get());
    cmd_manager_.list()->Close();
    cmd_manager_.execute();
    cmd_manager_.waitForGpu();
}

void Dx12::update(render::Camera &camera) {
    auto &curr_frame_res = frame_resources_.at(Commands::frameIndex());
    std::set<id_t>& dirty_transforms = core::scene.componentPool<core::Transform>().dirtyElements();
    std::set<id_t>& dirty_mats = core::scene.componentPool<core::Geometry>().dirtyElements();
    core::Pool<core::Geometry>& geometries = core::scene.componentPool<core::Geometry>();
    auto entity_with_new_geo = core::Entity(geometries.popNew());
    auto entity_with_removed_geo = geometries.popRemoved();

    Constant<GlobalShaderData> pass_constant;
    Constant<PerObjectData> obj_constant;
    Constant<Material> mat_constant;

    // update pass constants
    pass_constant.data.view_proj = math::transpose(camera.getViewProjectionMatrix());
    curr_frame_res.pass_buffer.copyData(0, &pass_constant);

    // update object constants
    for (auto id : dirty_transforms) {
        core::Transform trans { id } ;
        obj_constant.data.world_view_proj = trans.world();
        trans.unDirty();
        curr_frame_res.constant_buffer.copyData(id::index(id), &obj_constant);
    }

    // update material constants
    for (auto id : dirty_mats) {
        core::Geometry geo { id } ;
        mat_constant.data.base_color = geo.material().base_color;
        geo.unDirty();
        curr_frame_res.mat_buffer.copyData(id::index(geo.id()), &mat_constant);
    }

    // add new meshes
    while(entity_with_new_geo.isAlive()) {
        loadAsset(entity_with_new_geo);
        entity_with_new_geo = core::Entity(geometries.popNew());
    }

    while(entity_with_removed_geo != id::invalid) {
        gpass_.removeRenderElement(entity_with_removed_geo);
        entity_with_removed_geo = geometries.popRemoved();
    }

}

void Dx12::renderSurface() {
    auto &curr_frame_res = frame_resources_[Commands::frameIndex()];
    ID3D12GraphicsCommandList*command_list = cmd_manager_.list();

    cmd_manager_.reset(); //Resets commands list and current frame allocator

    clean_deferred_resources(heaps_); // Clean deferreds resources

    surface_.setViewport(command_list);

    auto target_barrier = CD3DX12_RESOURCE_BARRIER::Transition(
            curr_frame_res.back_buffer.Get(), D3D12_RESOURCE_STATE_PRESENT,
            D3D12_RESOURCE_STATE_RENDER_TARGET);
    command_list->ResourceBarrier(1, &target_barrier);

    gpass_.render(command_list, curr_frame_res);

    auto present_barrier = CD3DX12_RESOURCE_BARRIER::Transition(
            curr_frame_res.back_buffer.Get(),
            D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT );


#ifdef IMGUI
    ID3D12DescriptorHeap* srvDesc = heaps_.srv.get();
    command_list->SetDescriptorHeaps(1, &srvDesc);
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), command_list);
#endif

    command_list->ResourceBarrier(1, &present_barrier);
    command_list->Close() >> utl::DxCheck;
    cmd_manager_.execute();

#ifdef IMGUI
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault(nullptr, (void*)cmd_manager_.list());
#endif
    surface_.present();
    cmd_manager_.moveToNextFrame();
}

void Dx12::resize(const window::Resolution &res) {
    if (res.aspect_ratio == surface_.resolution().aspect_ratio) {
        return;
    }

    if (res.width == 0 and res.height == 0) {
        return;
    }

    cmd_manager_.waitForGpu();

    cmd_manager_.reset(nullptr);

    for(auto &frame_resource: frame_resources_){
        frame_resource.back_buffer.Reset();
    }

    depth_stencil_buffer_.Reset();
    surface_.resize(res);
    initDsBuffer();
    cmd_manager_.resetFences();

    for (u32 i = 0; i < frameBufferCount; ++i) {
        D3D12_RENDER_TARGET_VIEW_DESC desc = {
                .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
                .ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D
        };
        surface_.getBuffer(i, frame_resources_[i].back_buffer);
        device_->CreateRenderTargetView(frame_resources_[i].back_buffer.Get() ,&desc,
                                        frame_resources_[i].back_buffer_handle.cpu);
    }

    cmd_manager_.list()->Close();
    cmd_manager_.execute();
    cmd_manager_.waitForGpu();

    surface_.updateViewport();
}

void Dx12::terminate() {
#ifdef _DEBUG
    utl::queue_info(device_.Get(), FALSE);
    utl::set_reporter(device_.Get());
#endif
    cmd_manager_.flush();
    heaps_.release();
    gpass_.terminate();

    for (auto&frame_resource: frame_resources_) {
        frame_resource.constant_buffer.release();
        frame_resource.pass_buffer.release();
        frame_resource.mat_buffer.release();
    }
    clean_deferred_resources(heaps_);
}

void Dx12::removeAsset(id_t id) {
//    gpass_.removeRenderElement();
}


}