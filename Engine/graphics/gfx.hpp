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

#include "directX12/dx_graphics.hpp"
#include "vulkan/vk_graphics.hpp"

#include <concepts>

namespace reveal3d::graphics {

// Hardware Render Interface Concept
template<typename Gfx>
concept HRI = requires(Gfx graphics) {
    {graphics.LoadPipeline()} ->  std::same_as<void>;
    {graphics.LoadAssets()} ->  std::same_as<void>;
};
}

