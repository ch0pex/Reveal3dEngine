/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file mesh.hpp
 * @version 1.0
 * @date 10/03/2024
 * @brief mesh and Submesh
 *
 * Mesh, submesh and shader structures
 */

#pragma once

#include "material.hpp"
#include "vertex.hpp"


namespace reveal3d::render {

struct SubMesh {
  u32 vertex_pos {0};
  u32 index_pos {0};
  u32 index_count {0};
  bool visible {true};
  u8 shader {0};
};

struct Mesh {
  std::vector<Vertex> vertices {}; // Unique vertices
  std::vector<u32> indices {};
  u32 vertex_count {0}; // Count vertex with different position
  u32 triangle_count {0}; // Triangle count
};

auto constexpr is_visible = [](SubMesh const& mesh) { return mesh.visible; };

} // namespace reveal3d::render
