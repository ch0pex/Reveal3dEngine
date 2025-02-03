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
#include "core/entity.hpp"
#include "core/scene.hpp"
#include "resources/dx_buffer.hpp"
#include "resources/dx_frame_resource.hpp"
#include "utils/dx_defaults.hpp"

namespace reveal3d::graphics::dx12 {

class RenderElement {
public:
  RenderElement(core::Geometry const geo, ID3D12GraphicsCommandList* cmd_list) :
    vertex_buffer_(defaults::buffer1d(geo.vertices().size() * sizeof(render::Vertex))),
    index_buffer_(defaults::buffer1d(geo.indices().size() * sizeof(u32), DXGI_FORMAT_R32_UINT)),
    vertex_view_(vertex_view(vertex_buffer_)), index_view_(index_view(index_buffer_)), //
    entity_(geo.entity()) //
  {
    upload_resource(cmd_list, vertex_buffer_, geo.vertices());
    upload_resource(cmd_list, index_buffer_, geo.indices());
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

/*
class RenderElementDesc {

public:
  RenderElementDesc(core::Geometry const geo, RenderElement render_element) :
    vertex_view_(buffer_view<D3D12_VERTEX_BUFFER_VIEW>(render_element)),
    index_view_(buffer_view<D3D12_INDEX_BUFFER_VIEW>(index_buffer_)), //
    entity_(core::scene.getEntity(geo.entityIdx())) //
    private :

    D3D12_VERTEX_BUFFER_VIEW vertex_view_;
  D3D12_INDEX_BUFFER_VIEW index_view_;
  D3D_PRIMITIVE_TOPOLOGY topology_ {D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST};

  core::Entity entity_;
};
*/

} // namespace reveal3d::graphics::dx12
