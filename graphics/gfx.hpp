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
concept HRI = requires(Gfx graphics, render::Camera& camera, window::Resolution& res) {
  { graphics.loadPipeline() } -> std::same_as<void>;
  { graphics.loadAssets() } -> std::same_as<void>;
  { graphics.update(camera) } -> std::same_as<void>;
  { graphics.renderSurface(std::declval<typename Gfx::surface&>()) } -> std::same_as<void>;
  { graphics.terminate() } -> std::same_as<void>;
  { graphics.resize(res) } -> std::same_as<void>;
};

} // namespace reveal3d::graphics
