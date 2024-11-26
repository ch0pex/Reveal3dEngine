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

#include "core/components/geometry.hpp"
#include "core/components/transform.hpp"

namespace reveal3d::graphics {

using namespace render;
using namespace dx12;

Dx12::Dx12(window::Resolution const res) : gpass_(res, heaps_), surface_(res) { }

void Dx12::loadPipeline() { surface_.createSwapChain(cmd_manager_, heaps_); }

void Dx12::loadAssets() {
  cmd_manager_.reset(nullptr);
  using namespace core;

  auto& geometries = scene.componentPool<Geometry>();

  auto entity   = Entity(geometries.popNew());
  auto geometry = entity.component<core::Geometry>();

  while (geometry.isAlive()) {
    id_t const idx = id::index(geometry.id());
    auto transform = entity.component<Transform>();
    gpass_.addRenderElement(entity, cmd_manager_);
    Constant<PerObjectData> obj_constant;
    Constant<Material> mat_constant;
    for (auto& frame_resource: frame_resources_) {
      mat_constant.data.base_color          = geometry.material().base_color;
      frame_resource.per_obj_buffer.at(idx) = {.world_view_proj = transform.world(), .entity_id = entity.id()};
      frame_resource.mat_buffer.at(idx)     = geometry.material();
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
  gpass_.addRenderElement(id, cmd_manager_);
  cmd_manager_.list()->Close() >> utl::DxCheck;
  cmd_manager_.execute();
  cmd_manager_.waitForGpu();
}

void Dx12::update(Camera const& camera) {
  auto& [constant_buffer, pass_buffer, mat_buffer] = frame_resources_.at(Commands::frameIndex());
  auto const& dirty_transforms                     = core::scene.componentPool<core::Transform>().dirtyElements();
  auto const& dirty_mats                           = core::scene.componentPool<core::Geometry>().dirtyElements();
  auto& geometries                                 = core::scene.componentPool<core::Geometry>();
  auto new_geometry                                = core::Geometry(geometries.popNew());
  auto removed_geometry                            = core::Geometry(geometries.popRemoved());

  // update pass constants
  auto const view_proj = transpose(camera.getViewProjectionMatrix());
  pass_buffer.at(0)    = {
       .view          = camera.getViewMatrix(),
       .inv_view      = inverse(camera.getViewMatrix()),
       .proj          = camera.getProjectionMatrix(),
       .inv_proj      = inverse(camera.getProjectionMatrix()),
       .view_proj     = view_proj,
       .inv_view_proj = inverse(view_proj),
    // .eye_pos       = camera.position(),
       .near_z = config::camera.near_plane,
       .far_z  = config::camera.far_plane,
    // .total_time = ;
  };


  // update object constants
  for (auto const id: dirty_transforms) {
    core::Transform const trans {id};
    constant_buffer.at(id::index(id)) = {
      .world_view_proj = trans.world(), .entity_id = core::scene.getEntity(trans.entityIdx()).id() //
    };
    trans.unDirty();
  }

  // update material constants
  for (auto const id: dirty_mats) {
    core::Geometry const geo {id};
    mat_buffer.at(id::index(geo.id())) = geo.material();
    geo.unDirty();
  }

  // addId new meshes
  while (new_geometry.isAlive()) {
    loadAsset(core::scene.getEntity(new_geometry.entityIdx()));
    new_geometry = geometries.popNew();
  }

  while (removed_geometry.isAlive()) {
    gpass_.removeRenderElement(removed_geometry.id());
    removed_geometry = geometries.popRemoved();
  }
}

void Dx12::renderSurface(Surface& surface) {
  auto const& curr_frame_res = frame_resources_.at(Commands::frameIndex());
  auto* command_list         = cmd_manager_.list();
  auto* const back_buffer    = surface.back_buffer();

  cmd_manager_.reset(); // Resets commands list and current frame allocator

  clean_deferred_resources(heaps_); // Clean deferred resources

  surface.setViewport(command_list);

  auto const target_barrier = CD3DX12_RESOURCE_BARRIER::Transition(
      back_buffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET
  );

  command_list->ResourceBarrier(1, &target_barrier);

  gpass_.setRenderTargets(command_list, curr_frame_res, surface.rtv());

  gpass_.render(command_list, curr_frame_res);

  ImGuiBegin();

  auto const present_barrier = CD3DX12_RESOURCE_BARRIER::Transition(
      back_buffer, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT
  );

  command_list->ResourceBarrier(1, &present_barrier);

  command_list->Close() >> utl::DxCheck;
  cmd_manager_.execute();

  ImGuiEnd();

  surface.present();
  cmd_manager_.moveToNextFrame();
}

void Dx12::resize(window::Resolution const res) {
  if (res == surface_.resolution()) {
    return;
  }

  if (res.null()) {
    return;
  }

  cmd_manager_.waitForGpu();
  cmd_manager_.reset(nullptr);

  gpass_.resize(res, heaps_);
  surface_.resize(res);
  cmd_manager_.resetFences();

  cmd_manager_.list()->Close() >> utl::DxCheck;
  cmd_manager_.execute();
  cmd_manager_.waitForGpu();
}

void Dx12::terminate() {
#ifdef _DEBUG
  utl::queue_info(adapter.device.Get(), FALSE);
  utl::set_reporter(adapter.device.Get());
#endif

  cmd_manager_.flush();
  heaps_.release();
  gpass_.terminate();

  for (auto& frame_resource: frame_resources_) {
    frame_resource.per_obj_buffer.release();
    frame_resource.pass_buffer.release();
    frame_resource.mat_buffer.release();
  }
  clean_deferred_resources(heaps_);
}

} // namespace reveal3d::graphics
