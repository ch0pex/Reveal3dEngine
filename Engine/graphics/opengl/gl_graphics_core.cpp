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
    CreateContext();

    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("Error on OpenGl init\r\n");
    }
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glCullFace(GL_FRONT_AND_BACK);
//    glDisable(GL_CULL_FACE);

    renderLayers_.Init();
}

void Graphics::LoadAssets() {
    std::vector<core::Transform> &transforms = core::scene.Transforms();
    std::vector<core::Geometry> &geometries = core::scene.Geometries();

    for(u32 i = 0; i < core::scene.NumEntities(); ++i) {
        if (geometries[i].RenderInfo() == UINT_MAX) {

            renderElements_.emplace_back(geometries[i].Vertices(), geometries[i].Indices(), transforms[i].World());
            geometries[i].SetRenderInfo(i);

            for (auto &mesh : geometries[i].SubMeshes()) {
                mesh.renderInfo = i;
                mesh.constantIndex = i;
                renderLayers_.AddMesh(mesh);
            }
        } else {
            for (auto &mesh : geometries[i].SubMeshes()) {
                mesh.renderInfo = geometries[i].RenderInfo();
                mesh.constantIndex = i;
                renderLayers_.AddMesh(mesh);
            }
        }
    }
}

void Graphics::LoadAsset() {
    //TODO
}

void Graphics::Update(render::Camera &camera) {
    passConstant_ = camera.GetViewProjectionMatrix();

//    auto &transforms = core::scene.Transforms();
//
//    for (u32 i = 0; i < core::scene.NumEntities(); ++i) {
//        if (transforms[i].IsDirty() > 0) {
//            renderElements_[i].world = transforms[i].World();
//            transforms[i].UpdateDirty();
//        }
//    }
}

void Graphics::PrepareRender() {
    glClearColor(config::clearColor.x, config::clearColor.y, config::clearColor.z, config::clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Graphics::Draw() {
    for(u32 i = 0; i < render::shader::count; ++i) {
        renderLayers_.Draw(renderElements_, passConstant_,i);
    }
    SwapBuffer();
}

void Graphics::Terminate() {
    TerminateContext();
}

void Graphics::Resize(const window::Resolution &res) {

}

#ifdef WIN32

void Graphics::CreateContext() {
    window_.hdc = GetDC(window_.hwnd);

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

}

void Graphics::SwapBuffer() {
    SwapBuffers(window_.hdc);
}

void Graphics::TerminateContext() {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(window_.hglrc);
    ReleaseDC(window_.hwnd, window_.hdc);
}

#else

void Graphics::CreateContext() {
    glfwMakeContextCurrent(window_);
}

void Graphics::SwapBuffer() {
    glfwSwapBuffers(window_);
}

void Graphics::TerminateContext() {

}

#endif

}
