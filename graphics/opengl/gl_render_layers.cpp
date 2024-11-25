/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file gl_render_layers.cpp
 * @version 1.0
 * @date 10/04/2024
 * @brief Short description
 *
 * Longer description
 */

#include "gl_render_layers.hpp"
#include "core/scene.hpp"

#include <fstream>
#include <iostream>

namespace reveal3d::graphics::opengl {

void RenderLayers::init() {
  layers_[4].shaderId = createProgram(
      relative("Engine/graphics/opengl/shaders/solidShader.vert").c_str(),
      relative("Engine/graphics/opengl/shaders/solidShader.frag").c_str()
  );
  //    layers_[render::Shader::unlit].shaderId =
  //    createProgram(relative("Engine/graphics/opengl/shaders/flatShader.vert").c_str(),
  //                                                             relative("Engine/graphics/opengl/shaders/flatShader.frag").c_str());
}

std::string RenderLayers::readShader(char const* file_name) {
  std::string shader_code;
  std::ifstream file(file_name, std::ios::in);
  if (!file.good()) {
    logger(LogError) << "Could not read Shader..."s;
    std::terminate();
  }

  file.seekg(0, std::ios::end);
  shader_code.resize((unsigned int)file.tellg());
  file.seekg(0, std::ios::beg);
  file.read(shader_code.data(), shader_code.size());
  file.close();
  return shader_code;
}

u32 RenderLayers::createShader(GLenum shader_type, std::string& source, char const* shader_name) {
  i32 compile_result          = 0;
  u32 const shader            = glCreateShader(shader_type);
  char const* shader_code_ptr = source.c_str();
  int const shader_code_size  = source.size();
  glShaderSource(shader, 1, &shader_code_ptr, &shader_code_size);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result);

  if (compile_result == GL_FALSE) {
    i32 info_log_length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
    std::vector<char> shader_log(info_log_length);
    glGetShaderInfoLog(shader, info_log_length, nullptr, shader_log.data());
    logger(LogInfo) << "Error compiling Shader: " << shader_log.data();
    return 0;
  }
  return shader;
}

u32 RenderLayers::createProgram(char const* vs, char const* fs) {
  std::string source_vertex_shader   = readShader(vs);
  std::string source_fragment_shader = readShader(fs);

  u32 const vertex_shader   = createShader(GL_VERTEX_SHADER, source_vertex_shader, "Vertex Shader");
  u32 const fragment_shader = createShader(GL_FRAGMENT_SHADER, source_fragment_shader, "Fragment Shader");

  i32 link_result = 0;

  u32 const program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &link_result);

  if (link_result == GL_FALSE) {
    int info_log_length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
    std::vector<char> info_log(info_log_length);
    glGetProgramInfoLog(program, info_log_length, NULL, &info_log[0]);
    std::cout << "Shader Loader: LINK ERROR." << std::endl << &info_log[0] << std::endl;
    return 0;
  }
  return program;
}

void RenderLayers::addMesh(render::SubMesh& mesh) { sub_meshes_[3].push_back(&mesh); }

void RenderLayers::draw(std::vector<RenderElement>& render_elments, math::mat4& pass_constants, u32 layer) {
  /*
      const i32 vp_loc = glGetUniformLocation(layers_[layer].shaderId, "vp");
      const i32 model_loc = glGetUniformLocation(layers_[layer].shaderId, "model");
      const i32 ambient_color_loc = glGetUniformLocation(layers_[layer].shaderId, "ambientColor");
      const i32 ambient_light_intensity_loc = glGetUniformLocation(layers_[layer].shaderId, "ambientLightIntensity");
      const i32 sun_light_dir_loc = glGetUniformLocation(layers_[layer].shaderId, "sunLightDirection");
      const i32 sun_light_color_loc = glGetUniformLocation(layers_[layer].shaderId, "sunLightColor");
      const i32 sun_light_intensity_loc = glGetUniformLocation(layers_[layer].shaderId, "sunLightIntensity");

      glUseProgram(layers_[layer].shaderId);
      glUniform1f(ambient_light_intensity_loc, 0.7f);
      glUniform3f(ambient_color_loc, 1.0f, 1.0f, 1.0f);
      glUniform3f(sun_light_dir_loc, 0.0f, 0.5f, -1.0f);
      glUniform3f(sun_light_color_loc, 1.0f, 1.0f, 1.0f);
      glUniform1f(sun_light_intensity_loc, 0.9f);
      glUniformMatrix4fv(vp_loc, 1, GL_FALSE, (f32 *) &passConstants);

  //    glBindTexture(GL_TEXTURE_2D, texture_);

      for (const auto &mesh: subMeshes_[layer]) {
          if (!mesh->visible) continue;
          const math::mat4 world_mat =
  math::Transpose(core::scene.componentPool<core::Transform>().at(mesh->constantIndex).world_mat());
          glUniformMatrix4fv(model_loc, 1, GL_FALSE, (f32 *) &world_mat);
          glBindVertexArray(renderElments[mesh->renderInfo].vao);
          glDrawElements(GL_TRIANGLES, mesh->indexCount * 2, GL_UNSIGNED_INT, 0);
          glBindVertexArray(0);
      }
  }

  */

};

} // namespace reveal3d::graphics::opengl
