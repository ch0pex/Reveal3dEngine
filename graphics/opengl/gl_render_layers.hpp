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

#include "render/mesh.hpp"
#include "gl_render_info.hpp"

namespace reveal3d::graphics::opengl {

struct Layer {
    u32 shaderId;
};

class RenderLayers  {
public:
    void Init();
    void AddMesh(render::SubMesh &mesh);
    void Draw(std::vector<RenderElement>& renderElments, math::mat4 &passConstants, u32 layer);

    INLINE Layer& operator[] (u32 index) { return layers_[index]; }
    INLINE const Layer& operator[] (u32 index) const { return layers_[index]; }

private:
    static std::string ReadShader(const char* fileName);
    static u32 CreateShader(GLenum shaderType, std::string &source, const char* shaderName);
    static u32 CreateProgram(const char* vs, const char* fs);

    Layer layers_[render::Shader::count];
    std::vector<render::SubMesh *> subMeshes_[render::Shader::count];

};

}
