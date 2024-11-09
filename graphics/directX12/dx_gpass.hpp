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

#include "common/common.hpp"
#include "core/scene.hpp"
#include "dx_commands.hpp"
#include "dx_common.hpp"
#include "dx_pso.hpp"
#include "dx_render_info.hpp"
#include "resources/dx_resources.hpp"

namespace reveal3d::graphics::dx12 {

class Gpass {
public:
  Gpass();
  void init(ID3D12Device* device);
  static void setRenderTargets(Commands const& command_mng, FrameResource const& frame_resource);
  void depthPrepass(); // TODO
  void render(ID3D12GraphicsCommandList* command_list, FrameResource& frame_resource);
  void addRenderElement(core::Entity entity, Commands const& cmd_mng, ID3D12Device* device);
  void removeRenderElement(u32 const idx) { render_elements_.unordered_remove(idx); }
  void terminate();

private:
  void buildPsos(ID3D12Device* device);
  void buildRoots(ID3D12Device* device);
  void drawWorldGrid(ID3D12GraphicsCommandList* command_list, FrameResource& frame_resource);

  /**************** render elements *****************/
  reveal3d::utl::vector<RenderElement> render_elements_;

  /**************** Pipeline state and root signatures *****************/
  std::array<GraphicsPso, render::Shader::count> pipeline_states_;
  std::array<RootSignature, render::Shader::count> root_signatures_;

  ID3D12RootSignature* curr_root_signature_ {nullptr};
  ID3D12PipelineState* curr_pipeline_state_ {nullptr};
};


} // namespace reveal3d::graphics::dx12
