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

class alignas(16) Dx12 {
public:
  using surface = dx12::Surface;

  explicit Dx12(window::Resolution res);

  ~Dx12() = default;

  void loadPipeline();

  void loadAssets();

  void update(render::Camera const& camera);

  void render() { renderSurface(surface_); }

  void renderSurface(dx12::Surface& surface);

  void resize(window::Resolution res);

  void initWindow(WHandle const& win_handle) { surface_.setWindow(win_handle); }

  [[nodiscard]] ID3D12Device* device() const { return dx12::adapter.device.Get(); }

  dx12::Heaps const& heaps() { return heaps_; }

private:
  void imGuiBegin() const;

  void imGuiEnd() const;

  /****************** Frame resources *****************/
  dx12::utl::ResourceArray<dx12::FrameResource> frame_resources_;

  /******************* Gpu heaps and commands **********************/
  dx12::Heaps heaps_;
  dx12::Commands cmd_manager_;

  /******************* Gpass class **********************/
  dx12::Gpass gpass_;

  /***************** Surface and swapchain pos_rot_scale **********************/
  dx12::Surface surface_;
};

} // namespace reveal3d::graphics
