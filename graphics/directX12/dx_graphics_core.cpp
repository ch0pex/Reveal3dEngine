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
#include "IMGUI/backends/imgui_impl_dx12.h"
#include "IMGUI/imgui.h"
// #include "IMGUI/imgui_internal.h"
#endif

#include "config/config.hpp"
#include "core/components/geometry.hpp"
#include "core/components/transform.hpp"
#include "dx_graphics_core.hpp"
namespace reveal3d::graphics {

using namespace render;
using namespace dx12;

Dx12::Dx12(window::Resolution const* res) : surface_(*res) { }

void Dx12::loadPipeline() {
  cmd_manager_.init(adapter_.device.Get());
  heaps_.init(adapter_.device.Get());
  surface_.createSwapChain(cmd_manager_, adapter_, heaps_);
  frame_resources_.init(adapter_.device.Get());
  ds_handle_ = heaps_.dsv.alloc();
  initDsBuffer();
  gpass_.init(adapter_.device.Get());
}


void Dx12::initDsBuffer() {
  const D3D12_RESOURCE_DESC depth_stencil_desc = {
    .Dimension        = D3D12_RESOURCE_DIMENSION_TEXTURE2D,
    .Alignment        = 0,
    .Width            = surface_.resolution().width,
    .Height           = surface_.resolution().height,
    .DepthOrArraySize = 1,
    .MipLevels        = 1,
    .Format           = DXGI_FORMAT_R24G8_TYPELESS,
    .SampleDesc =
        {
          .Count   = 1,
          .Quality = 0,
        },
    .Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN,
    .Flags  = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL,
  };

  constexpr D3D12_CLEAR_VALUE opt_clear =
      {.Format       = DXGI_FORMAT_D24_UNORM_S8_UINT,
       .DepthStencil = {
         .Depth   = 1.0f,
         .Stencil = 0,
       }};

  auto const heap_prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
  adapter_.device->CreateCommittedResource(
      &heap_prop, D3D12_HEAP_FLAG_NONE, &depth_stencil_desc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &opt_clear,
      IID_PPV_ARGS(depth_stencil_buffer_.GetAddressOf())
  ) >> utl::DxCheck;

  constexpr D3D12_DEPTH_STENCIL_VIEW_DESC dsv_desc = {
    .Format        = DXGI_FORMAT_D24_UNORM_S8_UINT,
    .ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D,
    .Flags         = D3D12_DSV_FLAG_NONE,
    .Texture2D     = {.MipSlice = 0}
  };

  depth_stencil_buffer_->SetName(L"Depth Buffer") >> utl::DxCheck;
  adapter_.device->CreateDepthStencilView(depth_stencil_buffer_.Get(), &dsv_desc, ds_handle_.cpu);

  for (auto& frame_resource: frame_resources_) {
    frame_resource.depth_buffer_handle = ds_handle_;
  }
}


void Dx12::loadAssets() {
  cmd_manager_.reset(nullptr);
  using namespace core;

  auto& geometries = scene.componentPool<Geometry>();

  auto entity   = Entity(geometries.popNew());
  auto geometry = entity.component<core::Geometry>();

  while (geometry.isAlive()) {
    id_t const idx = id::index(geometry.id());
    auto transform = entity.component<Transform>();
    gpass_.addRenderElement(entity, cmd_manager_, adapter_.device.Get());
    Constant<PerObjectData> obj_constant;
    Constant<Material> mat_constant;
    for (auto& frame_resource: frame_resources_) {
      obj_constant.data.world_view_proj = transform.world();
      mat_constant.data.base_color      = geometry.material().base_color;
      frame_resource.constant_buffer.copyData(idx, &obj_constant, 1);
      frame_resource.mat_buffer.copyData(idx, &mat_constant, 1);
    }

    entity   = Entity(geometries.popNew());
    geometry = entity.component<core::Geometry>();
  }

  cmd_manager_.list()->Close();
  cmd_manager_.execute();
  cmd_manager_.waitForGpu();
}

void Dx12::loadAsset(core::Entity const id) {
  cmd_manager_.reset(nullptr);
  gpass_.addRenderElement(id, cmd_manager_, adapter_.device.Get());
  cmd_manager_.list()->Close() >> utl::DxCheck;
  cmd_manager_.execute();
  cmd_manager_.waitForGpu();
}

void Dx12::update(Camera const& camera) {
  auto& [back_buffer, back_buffer_handle, depth_buffer_handle, constant_buffer, pass_buffer, mat_buffer] =
      frame_resources_.at(Commands::frameIndex());
  auto const& dirty_transforms = core::scene.componentPool<core::Transform>().dirtyElements();
  auto const& dirty_mats       = core::scene.componentPool<core::Geometry>().dirtyElements();
  auto& geometries             = core::scene.componentPool<core::Geometry>();
  auto entity_with_new_geo     = core::Entity(geometries.popNew());
  auto entity_with_removed_geo = geometries.popRemoved();

  Constant<GlobalShaderData> pass_constant;
  Constant<PerObjectData> obj_constant;
  Constant<Material> mat_constant;

  // update pass constants
  pass_constant.data.view_proj = transpose(camera.getViewProjectionMatrix());
  pass_buffer.copyData(0, &pass_constant);

  // update object constants
  for (auto const id: dirty_transforms) {
    core::Transform trans {id};
    obj_constant.data.world_view_proj = trans.world();
    trans.unDirty();
    constant_buffer.copyData(id::index(id), &obj_constant);
  }

  // update material constants
  for (auto const id: dirty_mats) {
    core::Geometry geo {id};
    mat_constant.data.base_color = geo.material().base_color;
    geo.unDirty();
    mat_buffer.copyData(id::index(geo.id()), &mat_constant);
  }

  // addId new meshes
  while (entity_with_new_geo.isAlive()) {
    loadAsset(entity_with_new_geo);
    entity_with_new_geo = core::Entity(geometries.popNew());
  }

  while (entity_with_removed_geo != id::invalid) {
    gpass_.removeRenderElement(entity_with_removed_geo);
    entity_with_removed_geo = geometries.popRemoved();
  }
}

void Dx12::renderSurface() {
  auto& curr_frame_res                    = frame_resources_.at(Commands::frameIndex());
  ID3D12GraphicsCommandList* command_list = cmd_manager_.list();

  cmd_manager_.reset(); // Resets commands list and current frame allocator

  clean_deferred_resources(heaps_); // Clean deferreds resources

  surface_.setViewport(command_list);

  auto const target_barrier = CD3DX12_RESOURCE_BARRIER::Transition(
      curr_frame_res.back_buffer.Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET
  );

  command_list->ResourceBarrier(1, &target_barrier);

  gpass_.setRenderTargets(command_list, curr_frame_res);

  gpass_.render(command_list, curr_frame_res);


  ImGuiBegin();

  auto const present_barrier = CD3DX12_RESOURCE_BARRIER::Transition(
      curr_frame_res.back_buffer.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT
  );

  command_list->ResourceBarrier(1, &present_barrier);

  command_list->Close() >> utl::DxCheck;
  cmd_manager_.execute();

  ImGuiEnd();

  surface_.present();
  cmd_manager_.moveToNextFrame();
}

void Dx12::resize(window::Resolution const& res) {
  if (res.aspect_ratio == surface_.resolution().aspect_ratio) {
    return;
  }

  if (res.width == 0 and res.height == 0) {
    return;
  }

  cmd_manager_.waitForGpu();
  cmd_manager_.reset(nullptr);

  for (auto& frame_resource: frame_resources_) {
    frame_resource.back_buffer.Reset();
  }

  depth_stencil_buffer_.Reset();
  surface_.resize(res);
  initDsBuffer();
  cmd_manager_.resetFences();

  cmd_manager_.list()->Close() >> utl::DxCheck;
  cmd_manager_.execute();
  cmd_manager_.waitForGpu();
}

void Dx12::terminate() {
#ifdef _DEBUG
  utl::queue_info(adapter_.device.Get(), FALSE);
  utl::set_reporter(adapter_.device.Get());
#endif

  cmd_manager_.flush();
  heaps_.release();
  gpass_.terminate();

  for (auto& frame_resource: frame_resources_) {
    frame_resource.constant_buffer.release();
    frame_resource.pass_buffer.release();
    frame_resource.mat_buffer.release();
  }
  clean_deferred_resources(heaps_);
}

} // namespace reveal3d::graphics
