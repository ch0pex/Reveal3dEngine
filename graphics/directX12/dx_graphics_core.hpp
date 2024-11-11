/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_graphics.hpp
 * @version 1.0
 * @date 25/02/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include "dx_adapter.hpp"
#endif

#ifdef IMGUI
#include "IMGUI/backends/imgui_impl_dx12.h"
#include "IMGUI/imgui.h"
// #include "IMGUI/imgui_internal.h"
#endif

#include "core/scene.hpp"
#include "dx_commands.hpp"
#include "dx_common.hpp"
#include "dx_gpass.hpp"
#include "dx_surface.hpp"
#include "render/camera.hpp"
#include "resources/dx_resources.hpp"

namespace reveal3d::graphics {

__declspec(align(16)) class Dx12 {
public:
  using surface = dx12::Surface;

  explicit Dx12(window::Resolution const* res);

  void loadPipeline();

  void loadAssets();

  void loadAsset(core::Entity id);

  void update(render::Camera const& camera);

  void render() { renderSurface(surface_); }

  void renderSurface(dx12::Surface& surface);

  void terminate();

  void resize(window::Resolution const& res);

  void initWindow(WHandle const& win_handle) { surface_.setWindow(win_handle); }

  [[nodiscard]] static ID3D12Device* device() { return dx12::adapter.device.Get(); }

  dx12::Heaps& heaps() { return heaps_; }

private:
  void initDsBuffer();

  void ImGuiBegin() const {
#ifdef IMGUI
    auto* command_list            = cmd_manager_.list();
    ID3D12DescriptorHeap* srvDesc = heaps_.srv.get();
    command_list->SetDescriptorHeaps(1, &srvDesc);
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), command_list);
#endif
  }

  void ImGuiEnd() const {
#ifdef IMGUI
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault(nullptr, (void*)cmd_manager_.list());
#endif
  }

  /****************** Frame resources *****************/
  dx12::utl::ResourceArray<dx12::FrameResource> frame_resources_;

  /***************** Depth stencil buffer**********************/
  ComPtr<ID3D12Resource> depth_stencil_buffer_;
  dx12::DescriptorHandle ds_handle_;

  /******************* Gpu heaps and commands **********************/
  dx12::Heaps heaps_;
  dx12::Commands cmd_manager_;

  /******************* Gpass class **********************/
  dx12::Gpass gpass_;

  /***************** Surface and swapchain pos_rot_scale **********************/
  dx12::Surface surface_;
};

} // namespace reveal3d::graphics
