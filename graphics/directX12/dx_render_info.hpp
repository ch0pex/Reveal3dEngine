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

#include "core/components/geometry.hpp"
#include "core/scene.hpp"
#include "resources/dx_resources.hpp"

namespace reveal3d::graphics::dx12 {

class RenderElement {
public:
  RenderElement(core::Entity const entity, ID3D12GraphicsCommandList* cmd_list) : entity_(entity) {
    auto const geo         = entity.component<core::Geometry>();
    auto const vertex_info = Buffer::buffer1d(cmd_list, geo.vertices().size() * sizeof(render::Vertex));
    auto const index_info  = Buffer::buffer1d(cmd_list, geo.indices().size() * sizeof(u32), DXGI_FORMAT_R32_UINT);

    vertex_buffer_.init(vertex_info, geo.vertices());
    index_buffer_.init(index_info, geo.indices());

    vertex_view_ = vertex_buffer_.view<D3D12_VERTEX_BUFFER_VIEW>();
    index_view_  = index_buffer_.view<D3D12_INDEX_BUFFER_VIEW>();
  }

  [[nodiscard]] constexpr auto const& vertices() const { return vertex_view_; }

  [[nodiscard]] constexpr auto const& indices() const { return index_view_; }

  [[nodiscard]] constexpr D3D_PRIMITIVE_TOPOLOGY topology() const { return topology_; }

  [[nodiscard]] core::Entity entity() const { return entity_; }

  ~RenderElement() { logger(LogInfo) << "Releasing render element of entity with id" << entity_.id(); }

private:
  //******************* Buffers ********************
  Buffer vertex_buffer_;
  Buffer index_buffer_;

  //******************* Buffers views **************
  D3D12_VERTEX_BUFFER_VIEW vertex_view_;
  D3D12_INDEX_BUFFER_VIEW index_view_;
  D3D_PRIMITIVE_TOPOLOGY topology_ {D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST};

  core::Entity entity_;
};


} // namespace reveal3d::graphics::dx12
