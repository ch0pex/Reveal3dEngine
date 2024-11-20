/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file graphics_core.hpp
 * @version 1.0
 * @date 05/04/2024
 * @brief Short description
 *
 * Longer description
 */
#pragma once

#include "gl_render_info.hpp"
#include "render/camera.hpp"

#include "gl_render_layers.hpp"

namespace reveal3d::graphics {

struct Surface { };

class OpenGL {
public:
  using surface = Surface;

  explicit OpenGL(window::Resolution* res);

  void loadPipeline();

  void loadAssets();

  void loadAsset();

  void update(render::Camera const& camera);

  void render() { }

  void renderSurface(surface& surface);

  void terminate() { }

  void resize(window::Resolution const& res);

  void initWindow(WHandle const& w_handle) { window_ = w_handle; }

private:
  void createContext();

  void swapBuffer() const;

  void terminateContext() const;

  math::mat4 pass_constant_;
  std::vector<opengl::RenderElement> render_elements_;
  opengl::RenderLayers render_layers_;
  WHandle window_ {};
};

} // namespace reveal3d::graphics
