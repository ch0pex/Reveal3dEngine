/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file gl_render_layers.hpp
 * @version 1.0
 * @date 10/04/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "gl_render_info.hpp"
#include "render/mesh.hpp"

#include <GL/glew.h>

namespace reveal3d::graphics::opengl {

struct Layer {
  u32 shaderId;
};

class RenderLayers {
public:
  void init();
  void addMesh(render::SubMesh& mesh);
  void draw(std::vector<RenderElement>& render_elments, math::mat4& pass_constants, u32 layer);

  Layer& operator[](u32 index) { return layers_[index]; }
  Layer const& operator[](u32 index) const { return layers_[index]; }

private:
  static std::string readShader(char const* file_name);
  static u32 createShader(GLenum shader_type, std::string& source, char const* shader_name);
  static u32 createProgram(char const* vs, char const* fs);

  Layer layers_[4];
  std::array<std::vector<render::SubMesh*>, 4> sub_meshes_;
};

} // namespace reveal3d::graphics::opengl
