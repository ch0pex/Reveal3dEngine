/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_render_info.hpp
 * @version 1.0
 * @date 27/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "core/scene.hpp"
#include "resources/dx_resources.hpp"

namespace reveal3d::graphics {

struct RenderElement
{
  RenderElement(const core::Entity entity, dx12::BufferInitInfo& vertex_info, dx12::BufferInitInfo& index_info) :
    entity(entity), vertex_buffer(vertex_info), index_buffer(index_info) { }
  D3D_PRIMITIVE_TOPOLOGY topology {D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST};
  dx12::VertexBuffer vertex_buffer;
  dx12::IndexBuffer index_buffer;
  core::Entity entity;
  //    u32 constantIndex;
};


} // namespace reveal3d::graphics
