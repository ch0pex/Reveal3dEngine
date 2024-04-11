/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file gl_render_layers.cpp
 * @version 1.0
 * @date 10/04/2024
 * @brief Short description
 *
 * Longer description
 */

#include "gl_render_layers.hpp"

#include <fstream>

namespace reveal3d::graphics::opengl {

void RenderLayers::Init() {
    layers_[render::shader::opaque].shaderId = CreateProgram(relative("Engine/graphics/opengl/shaders/solidShader.vs").c_str(),
                                                           relative("Engine/graphics/opengl/shaders/solidShader.fs").c_str());
    layers_[render::shader::flat].shaderId = CreateProgram(relative("Engine/graphics/opengl/shaders/flatShader.vs").c_str(),
                                                             relative("Engine/graphics/opengl/shaders/flatShader.fs").c_str());
}

std::string RenderLayers::ReadShader(const char* fileName) {
    std::string shader_code;
    std::ifstream file(fileName, std::ios::in);
    if (!file.good()) {
        log(logERROR) << "Could not read shader...";
        std::terminate();
    }

    file.seekg(0, std::ios::end);
    shader_code.resize((unsigned int) file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(shader_code.data(), shader_code.size());
    file.close();
    return shader_code;
}

u32 RenderLayers::CreateShader(GLenum shaderType, std::string &source, const char *shaderName) {
    i32 compile_result = 0;
    const u32 shader = glCreateShader(shaderType);
    const char *shader_code_ptr = source.c_str();
    const int shader_code_size = source.size();
    glShaderSource(shader, 1, &shader_code_ptr, &shader_code_size);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result);

    if (compile_result == GL_FALSE) {
        i32 info_log_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
        std::vector<char> shader_log(info_log_length);
        glGetShaderInfoLog(shader, info_log_length, NULL, &shader_log[0]);
        log(logDEBUG) << "Error compiling shader";
        return 0;
    }
    return shader;
}

u32 RenderLayers::CreateProgram(const char *vs, const char *fs) {
    std::string source_vertex_shader = ReadShader(vs);
    std::string source_fragment_shader = ReadShader(fs);

    const u32 vertex_shader = CreateShader(GL_VERTEX_SHADER, source_vertex_shader, "Vertex Shader");
    const u32 fragment_shader = CreateShader(GL_FRAGMENT_SHADER, source_fragment_shader, "Fragment Shader");

    int link_result = 0;

    const u32 program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &link_result);

    if (link_result == GL_FALSE) {
        int info_log_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
        std::vector<char> info_log(info_log_length);
        glGetProgramInfoLog(program, info_log_length, NULL, &info_log[0]);
        std::cout << "Shader Loader: LINK ERROR." << std::endl << &info_log[0] << std::endl;
        return 0;
    }
    return program;
}

void RenderLayers::AddMesh(render::SubMesh &mesh) {
    meshes_[mesh.shader].push_back(&mesh);
}

void RenderLayers::Draw(std::vector<RenderInfo> &renderElments, u32 layer) {

    u32 vp_loc = glGetUniformLocation(layers_[layer].shaderId, "vp");
    u32 ambient_color_loc = glGetUniformLocation(layers_[layer].shaderId, "ambientColor");
    u32 ambient_light_intensity = glGetUniformLocation(layers_[layer].shaderId, "ambientLightIntensity");
    u32 sun_light_dir_loc = glGetUniformLocation(layers_[layer].shaderId, "sunLightDirection");
    u32 sun_light_color_loc = glGetUniformLocation(layers_[layer].shaderId, "sunLightColor");
    u32 sun_light_intensity_loc = glGetUniformLocation(layers_[layer].shaderId, "sunLightIntensity");

    glUseProgram(layers_[layer].shaderId);
    glUniformMatrix4fv(vp_loc, 1, GL_FALSE, );
    glUniform1f(ambient_light_intensity, kGameConfig.ambient_light_intensity * 0.01f);
    glUniform3f(ambient_color_loc, kGameConfig.ambient_light_color.x, kGameConfig.ambient_light_color.y,
                kGameConfig.ambient_light_color.z);
    glUniform3f(sun_light_dir_loc, kGameConfig.sun_light_direction.x, kGameConfig.sun_light_direction.y,
                kGameConfig.sun_light_direction.z);
    glUniform3f(sun_light_color_loc, kGameConfig.sun_light_color.x, kGameConfig.sun_light_color.y,
                kGameConfig.sun_light_color.z);
    glUniform1f(sun_light_intensity_loc, kGameConfig.sun_light_intensity * 0.01f);

    glBindTexture(GL_TEXTURE_2D, texture_);

    for (const auto &mesh: meshes_) {
        glBindVertexArray(mesh->vao);
        glDrawElements(GL_TRIANGLES, mesh->faces * 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

}



};

