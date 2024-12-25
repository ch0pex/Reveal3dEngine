/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_gpass.hpp
 * @version 1.0
 * @date 12/08/2024
 * @brief Gpass main class
 *
 * This class will handle deferred render or forward+ in a future
 * for now it's just a simple forward rendering
 */

#pragma once

#include "../shaders.hpp"
#include "common/common.hpp"
#include "core/scene.hpp"
#include "dx_commands.hpp"
#include "dx_pso.hpp"
#include "dx_render_info.hpp"
#include "resources/dx_depth_buffer.hpp"
#include "resources/dx_frame_resource.hpp"

namespace reveal3d::graphics::dx12 {

class Gpass {
public:
  explicit Gpass(window::Resolution resolution, Heaps& heaps);

  ~Gpass() { logger(LogInfo) << "Releasing G-buffer and depth buffer resources"; }

  void setRenderTargets(
      ID3D12GraphicsCommandList* command_list, FrameResource const& frame_resource,
      D3D12_CPU_DESCRIPTOR_HANDLE const back_buffer
  ) const {

    command_list->ClearRenderTargetView(
        back_buffer, math::utils::to_array(config::scene.clearColor).data(), 0, nullptr
    );
    command_list->ClearDepthStencilView(
        depth_buffer_.dsv(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0F, 0, 0, nullptr
    );
    command_list->OMSetRenderTargets(1, &back_buffer, TRUE, &depth_buffer_.dsv());
  }

  void depthPrepass() { } // TODO

  void render(ID3D12GraphicsCommandList* command_list, FrameResource const& frame_resource);

  void addRenderElement(core::Geometry geo, Commands const& cmd_mng);

  void removeRenderElement(u32 const idx) { render_elements_.unordered_remove(id::index(idx)); }

  void resize(window::Resolution const res) { depth_buffer_.resize(res); }

private:
  void buildPsos();

  void buildRoots();

  void drawWorldGrid(ID3D12GraphicsCommandList* command_list, FrameResource const& frame_resource) const;

  /******************** Buffers *********************/
  DepthBuffer depth_buffer_;
  // GBuffer g_buffer_;

  /**************** render elements *****************/
  reveal3d::utl::vector<RenderElement> render_elements_;

  /**************** Pipeline state and root signatures *****************/
  std::array<GraphicsPso, Shader::count> pipeline_states_;
  std::array<RootSignature, Shader::count> root_signatures_;

  ID3D12RootSignature* curr_root_signature_ {nullptr};
  ID3D12PipelineState* curr_pipeline_state_ {nullptr};
};


} // namespace reveal3d::graphics::dx12
