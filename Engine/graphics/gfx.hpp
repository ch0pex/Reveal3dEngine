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

// Hardware Render Interface Concept
template<typename Gfx>
concept HRI = requires(Gfx graphics, render::Camera& camera, window::Resolution& res) {
    {graphics.LoadPipeline()} ->  std::same_as<void>;
    {graphics.LoadAssets()} ->  std::same_as<void>;
//    {graphics.LoadAsset(std::declval<u32>)} ->  std::same_as<void>;
    {graphics.Update(camera)} ->  std::same_as<void>;
    {graphics.PrepareRender()} ->  std::same_as<void>;
    {graphics.Draw()} ->  std::same_as<void>;
    {graphics.Terminate()} ->  std::same_as<void>;
    {graphics.Resize(res)} ->  std::same_as<void>;
};

}

