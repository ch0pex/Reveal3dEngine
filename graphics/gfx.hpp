/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file gfx.hpp
 * @version 1.0
 * @date 25/02/2024
 * @brief Short description
 *
 * Main graphics include file, it has the concept for a grahpics render interface
 *
 */

#pragma once
#ifdef WIN32
#include "directX12/dx_graphics_core.hpp"
#endif
#include "opengl/gl_graphics_core.hpp"
#include "vulkan/vk_graphics_core.hpp"

#include <concepts>

namespace reveal3d::graphics {

enum class Backends : uint8_t { directx12 = 0, directx11, openGL, vulkan, metal };

// Hardware render Interface Concept
template<typename Gfx>
concept HRI = requires(Gfx graphics, core::Scene& scene) {
  { graphics.loadPipeline() } -> std::same_as<void>;
  { graphics.loadAssets(scene) } -> std::same_as<void>;
  { graphics.update(scene, std::declval<render::Camera>()) } -> std::same_as<void>;
  { graphics.renderSurface(std::declval<typename Gfx::surface&>()) } -> std::same_as<void>;
  // { graphics.terminate() } -> std::same_as<void>;
  { graphics.resize(std::declval<window::Resolution>()) } -> std::same_as<void>;
};

#ifdef WIN32
static_assert(HRI<Dx12>);
#endif

static_assert(HRI<OpenGL>);

} // namespace reveal3d::graphics
