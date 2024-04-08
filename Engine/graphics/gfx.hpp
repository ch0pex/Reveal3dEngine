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
#include "directX12/graphics_core.hpp"
#endif
#include "opengl/graphics_core.hpp"
#include "vulkan/graphics_core.hpp"

#include <concepts>

namespace reveal3d::graphics {

// Hardware Render Interface Concept
template<typename Gfx>
concept HRI = requires(Gfx graphics) {
    {graphics.LoadPipeline()} ->  std::same_as<void>;
//    {graphics.LoadAssets(core::Scene&)} ->  std::same_as<void>;
};

}

