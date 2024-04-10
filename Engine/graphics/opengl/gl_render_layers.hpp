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

#include "GL/glew.h"
#include "render/mesh.hpp"

namespace reveal3d::graphics::opengl {

struct Layer {
    u32 shaderId;
};

class RenderLayers  {
public:
    void Init();
private:
    static std::string ReadShader(const char* fileName);
    static u32 CreateShader(GLenum shaderType, std::string &source, const char* shaderName);
    static u32 CreateProgram(const char* vs, const char* fs);

    Layer layers_[render::shader::count];
    std::vector<render::SubMesh *> meshes_[render::shader::count];

};

}
