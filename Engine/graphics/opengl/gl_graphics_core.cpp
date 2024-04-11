/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file graphics_core.cpp
 * @version 1.0
 * @date 05/04/2024
 * @brief Short description
 *
 * Longer description
 */

#include "gl_graphics_core.hpp"

#include "core/entity.hpp"
#include "config/config.hpp"

namespace reveal3d::graphics::opengl {

Graphics::Graphics(window::Resolution *res) {

}

void Graphics::LoadPipeline() {
    window_.hdc = GetDC(window_.handle);

    // Configuración del formato de píxel
    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;

    i32 format = ChoosePixelFormat(window_.hdc, &pfd);
    SetPixelFormat(window_.hdc, format, &pfd);

    // Creación del contexto de renderizado
    window_.hglrc = wglCreateContext(window_.hdc);
    wglMakeCurrent(window_.hdc, window_.hglrc);

    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("Error on OpenGl init\r\n");
    }
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glEnable(GL_DEPTH_TEST);

    renderLayers_.Init();
}

void Graphics::LoadAssets() {
    std::vector<core::Transform> &transforms = core::scene.Transforms();
    std::vector<core::Geometry> &geometries = core::scene.Geometries();

    for(u32 i = 0; i < core::scene.NumEntities(); ++i) {
        if (geometries[i].RenderInfo() == UINT_MAX) {
            for (auto &mesh : geometries[i].Meshes()) {
                mesh.renderInfo = i;
                renderLayers_.AddMesh(mesh);
            }
        } else {
            for (auto &mesh : geometries[i].Meshes()) {
                mesh.renderInfo = geometries[i].RenderInfo();
                renderLayers_.AddMesh(mesh);
            }
        }

    }

}

void Graphics::LoadAsset() {

}

void Graphics::Update(render::Camera &camera) {
    passConstant_ = camera.GetViewProjectionMatrix();

    passConstant.data.viewProj = math::Transpose(camera.GetViewProjectionMatrix());
    currFrameRes.passBuffer.CopyData(0, &passConstant);

    auto &transforms = core::scene.Transforms();

    for (u32 i = 0; i < core::scene.NumEntities(); ++i) {
        if (transforms[i].IsDirty() > 0) {
            objConstant.data.world = transforms[i].World();
            transforms[i].UpdateDirty();
            currFrameRes.constantBuffer.CopyData(i, &objConstant);
        }
    }
}

void Graphics::PrepareRender() {
    glClearColor(config::clearColor.x, config::clearColor.y, config::clearColor.z, config::clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Graphics::Draw() {
    for(u32 i = 0; i < render::shader::count; ++i) {
       renderLayers_.Draw(renderElements_, i);
    }
    SwapBuffers(window_.hdc);
}

void Graphics::Terminate() {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(window_.hglrc);
    ReleaseDC(window_.handle, window_.hdc);
}

void Graphics::Resize(const window::Resolution &res) {

}

}
