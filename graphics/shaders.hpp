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

#include "../common/common.hpp"


namespace reveal3d::graphics {

class Shader {
public:
  enum Type : u8 {
    Flat,
    Opaque,
    Grid,

    count
  };

private:
};

class ShaderCompiler { };


inline std::vector<Shader> compileShaders() { }


} // namespace reveal3d::graphics
