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
#include "utils/imgui.hpp"

namespace reveal3d::graphics {

using namespace render;
using namespace dx12;

Dx12::Dx12(window::Resolution const res) :
  gpass_(res, heaps_), surface_(res)
// texture_(heaps_.alloc<RenderTexture>(
// defaults::texture2d({1920, 1080}, DXGI_FORMAT_R8G8B8A8_UNORM, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET)
{ }

void Dx12::loadPipeline() { surface_.createSwapChain(cmd_manager_, heaps_); }

void Dx12::loadAssets(core::Scene& scene) {
  auto& geometry_pool = scene.pool<core::Geometry>();

  if (auto new_geo = core::Geometry {&scene, geometry_pool.popNew()}; new_geo.isAlive()) {
    cmd_manager_.reset();

    for (; new_geo.isAlive(); new_geo = {&scene, geometry_pool.popNew()}) {
      gpass_.addRenderElement(new_geo, cmd_manager_);
    }

    cmd_manager_.list()->Close() >> utils::DxCheck;
    cmd_manager_.execute();
    cmd_manager_.waitForGpu();
  }
}

void Dx12::update(core::Scene& scene, render::Camera const& camera) {
  auto& geometries = scene.pool<core::Geometry>();

  // Clean deferred resources of the current frame
  clean_deferred_resources();
  heaps_.cleanDeferreds();

  // Updates all resources for current frame
  frame_resources_.at(Commands::frameIndex()).update(scene, camera);

  // Load new meshes to gpu
  loadAssets(scene);

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


  surface.setViewport(command_list);

  // Setting resource to render target state
  auto const target_barrier = CD3DX12_RESOURCE_BARRIER::Transition(
      back_buffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET
  );

  command_list->ResourceBarrier(1, &target_barrier);

  // *** Setting target and rendering to it ***
  gpass_.setRenderTargets(command_list, curr_frame_res, surface.rtv());
  gpass_.render(command_list, curr_frame_res);

  imgui_start(command_list, heaps_.heap<HeapType::Srv>().get());

  // Setting resource to present state
  auto const present_barrier = CD3DX12_RESOURCE_BARRIER::Transition(
      back_buffer, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT
  );

  command_list->ResourceBarrier(1, &present_barrier);

  // Executing commands
  command_list->Close() >> utils::DxCheck;
  cmd_manager_.execute();

  imgui_end(command_list);

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
  surface_.resize(res, heaps_);

  // Resetting commander and executing commands
  cmd_manager_.resetFences();
  cmd_manager_.list()->Close() >> utils::DxCheck;
  cmd_manager_.execute();
  cmd_manager_.waitForGpu();
}

} // namespace reveal3d::graphics
