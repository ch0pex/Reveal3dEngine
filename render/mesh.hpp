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
 * mesh, submesh and shader structures
 */

#pragma once

#include "material.hpp"
#include "vertex.hpp"

#include <vector>

namespace reveal3d::render {

struct SubMesh {
  Shader shader {Opaque};
  u32 vertex_pos {0};
  u32 index_pos {0};
  u32 index_count {0};
  bool visible {true};
};

struct Mesh {
  std::vector<Vertex> vertices;
  std::vector<u32> indices;
};

} // namespace reveal3d::render
