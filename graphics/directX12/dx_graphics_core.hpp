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

#include "dx_commands.hpp"
#include "dx_gpass.hpp"
#include "dx_surface.hpp"
#include "render/camera.hpp"
#include "resources/dx_frame_resource.hpp"
#include "utils/dx_resource_array.hpp"

namespace reveal3d::graphics {

class alignas(16) Dx12 {
public:
  using surface = dx12::Surface;

  explicit Dx12(window::Resolution res);

  void loadPipeline();

  void loadAssets(core::Scene& scene);

  void update(core::Scene& scene, render::Camera const& camera);

  void render() { renderSurface(surface_); }

  void renderSurface(dx12::Surface& surface);

  void resize(window::Resolution res);

  void initWindow(WHandle const& win_handle) { surface_.setWindow(win_handle); }

  [[nodiscard]] ID3D12Device* device() const { return dx12::adapter.device.Get(); }

  dx12::Heaps const& heaps() { return heaps_; }

private:
  /****************** Frame resources *****************/
  dx12::utils::ResourceArray<dx12::FrameResource> frame_resources_;

  /******************* Gpu views heaps and commands **********************/
  dx12::Heaps heaps_;
  dx12::Commands cmd_manager_;

  /******************* Gpass class **********************/
  dx12::Gpass gpass_;

  /***************** Surface and swapchain pos_rot_scale **********************/
  dx12::Surface surface_;
  // dx12::RenderTexture texture_;
};

} // namespace reveal3d::graphics
