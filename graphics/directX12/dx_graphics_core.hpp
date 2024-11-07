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
#endif

#include <array>

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
  explicit Dx12(window::Resolution* res);
  void loadPipeline();
  void loadAssets();
  void loadAsset(core::Entity id);
  void update(render::Camera const& camera);
  void renderSurface();
  void terminate();
  void resize(window::Resolution const& res);
  void initWindow(WHandle const& win_handle) { surface_.setWindow(win_handle); }
  ID3D12Device* device() const { return device_.Get(); }
  dx12::Heaps& heaps() { return heaps_; }

private:
  void initDxgiAdapter();
  void initFrameResources();
  void initDsBuffer();
  void initConstantBuffers();
  void removeAsset(id_t id);

  /****************** Factory and Device *****************/
  ComPtr<IDXGIFactory5> factory_;
  ComPtr<ID3D12Device> device_;

  /****************** Frame resources *****************/
  std::array<dx12::FrameResource, config::Graphics::max_buffer_count> frame_resources_;

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
