/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_shaders.hpp
 * @version 1.0
 * @date 25/11/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "common/common.hpp"


namespace reveal3d::graphics {

enum class EngineShader : u8 {
  Flat = 0,
  Opaque,
  Grid,

  count
};

inline std::vector<EngineShader> compileShaders() { }


} // namespace reveal3d::graphics
