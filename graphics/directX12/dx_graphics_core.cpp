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
  using namespace core;
  auto& geometry_pool = scene.pool<Geometry>();

  if (Geometry new_geo = geometry_pool.popNew(); new_geo.isAlive()) {
    cmd_manager_.reset();

    for (; new_geo.isAlive(); new_geo = geometry_pool.popNew()) {
      gpass_.addRenderElement(new_geo, cmd_manager_);
    }

    cmd_manager_.list()->Close() >> utils::DxCheck;
    cmd_manager_.execute();
    cmd_manager_.waitForGpu();
  }
}

void Dx12::update(Camera const& camera) {
  auto& [constant_buffer, pass_buffer, mat_buffer] = frame_resources_.at(Commands::frameIndex());
  auto& geometries                                 = core::scene.pool<core::Geometry>();

  // update pass constants
  auto const view_proj = transpose(camera.getViewProjectionMatrix());
  pass_buffer.at(0)    = {
       .view          = camera.getViewMatrix(),
       .inv_view      = inverse(camera.getViewMatrix()),
       .proj          = camera.getProjectionMatrix(),
       .inv_proj      = inverse(camera.getProjectionMatrix()),
       .view_proj     = view_proj,
       .inv_view_proj = inverse(view_proj),
       .eye_pos       = camera.position(),
       .near_z        = camera.nearPlane(),
       .far_z         = camera.farPlane(),
  };

  // update object constants
  for (auto const id: core::scene.pool<core::Transform>().dirtyElements()) {
    core::Transform const trans {id};
    constant_buffer.at(id::index(id)) = {
      .world_view_proj = trans.world(), .entity_id = core::scene.getEntity(trans.entityIdx()).id() //
    };
    trans.unDirty();
  }

  // update material constants
  for (auto const id: geometries.dirtyElements()) {
    core::Geometry const geo {id};
    mat_buffer.at(id::index(geo.id())) = geo.material();
    geo.unDirty();
  }

  // Load new meshes to gpu
  loadAssets();

  // Remove render elements of deleted geometry
  for (id_t rem_geo = geometries.popRemoved(); id::is_valid(rem_geo); rem_geo = geometries.popRemoved()) {
    gpass_.removeRenderElement(rem_geo);
  }
}

void Dx12::renderSurface(Surface& surface) {
  auto const& curr_frame_res = frame_resources_.at(Commands::frameIndex());
  auto* command_list         = cmd_manager_.list();
  auto* const back_buffer    = surface.back_buffer();

  cmd_manager_.reset(); // Resets commands list and current frame allocator

  // *** Deferred resource cleaning ***
  clean_deferred_resources();
  heaps_.cleanDeferreds();

  surface.setViewport(command_list);

  // Setting resource to render target state
  auto const target_barrier = CD3DX12_RESOURCE_BARRIER::Transition(
      back_buffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET
  );

  command_list->ResourceBarrier(1, &target_barrier);

  // *** Setting target and rendering to it ***
  gpass_.setRenderTargets(command_list, curr_frame_res, surface.rtv());
  gpass_.render(command_list, curr_frame_res);

  imGuiBegin();

  // Setting resource to present state
  auto const present_barrier = CD3DX12_RESOURCE_BARRIER::Transition(
      back_buffer, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT
  );

  command_list->ResourceBarrier(1, &present_barrier);

  // Executing commands
  command_list->Close() >> utils::DxCheck;
  cmd_manager_.execute();

  imGuiEnd();

  // Presenting frame
  surface.present();
  cmd_manager_.moveToNextFrame();
}

void Dx12::resize(window::Resolution const res) {
  // Check we are not resizing to current res or (0,0)
  if (res == surface_.resolution() or res.isNull()) {
    return;
  }

  // Gpu and Cpu sync
  cmd_manager_.waitForGpu();
  cmd_manager_.reset(nullptr);

  // Resizing rtv and g-buffer
  gpass_.resize(res);
  surface_.resize(res);

  // Resetting commander and executing commands
  cmd_manager_.resetFences();
  cmd_manager_.list()->Close() >> utils::DxCheck;
  cmd_manager_.execute();
  cmd_manager_.waitForGpu();
}
void Dx12::imGuiBegin() const {
#ifdef IMGUI
  auto* command_list            = cmd_manager_.list();
  ID3D12DescriptorHeap* srvDesc = heaps_.srv.get();
  command_list->SetDescriptorHeaps(1, &srvDesc);
  ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), command_list);
#endif
}
void Dx12::imGuiEnd() const {
#ifdef IMGUI
  ImGui::UpdatePlatformWindows();
  ImGui::RenderPlatformWindowsDefault(nullptr, cmd_manager_.list());
#endif
}

} // namespace reveal3d::graphics
