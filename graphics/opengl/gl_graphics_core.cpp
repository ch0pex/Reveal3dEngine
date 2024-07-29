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

#include "core/scene.hpp"
#include "config/config.hpp"
//#ifdef WIN32
//#include <GL/wglew.h>
//#else
//#endif

namespace reveal3d::graphics {

using namespace opengl;

OpenGL::OpenGL(window::Resolution *res) {

}

void OpenGL::LoadPipeline() {
    CreateContext();

    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("Error on OpenGl init\r\n");
    }
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
//    wglSwapIntervalEXT(0);
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glCullFace(GL_FRONT_AND_BACK);
//    glDisable(GL_CULL_FACE);
    renderLayers_.Init();
}

void OpenGL::LoadAssets() {
    //TODO
//    std::vector<core::Transform> &transforms = core::scene.Transforms();
//    std::vector<core::Geometry> &geometries = core::scene.Geometries();
//
//    for(u32 i = 0; i < core::scene.NumEntities(); ++i) {
//        if (geometries[i].RenderInfo() == UINT_MAX) {
//
//            renderElements_.emplace_back(geometries[i].Vertices(), geometries[i].Indices(), transforms[i].World());
//            geometries[i].SetRenderInfo(i);
//
//            for (auto &mesh : geometries[i].SubMeshes()) {
//                mesh.renderInfo = i;
//                mesh.constantIndex = i;
//                renderLayers_.AddMesh(mesh);
//            }
//        } else {
//            for (auto &mesh : geometries[i].SubMeshes()) {
//                mesh.renderInfo = geometries[i].RenderInfo();
//                mesh.constantIndex = i;
//                renderLayers_.AddMesh(mesh);
//            }
//        }
//    }
}

void OpenGL::LoadAsset() {
    //TODO
}

void OpenGL::Update(render::Camera &camera) {
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

void OpenGL::PrepareRender() {
    glClearColor(config::clearColor.x, config::clearColor.y, config::clearColor.z, config::clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGL::Draw() {
    for(u32 i = 0; i < render::Shader::count; ++i) {
        renderLayers_.Draw(renderElements_, passConstant_,i);
    }
    SwapBuffer();
}

void OpenGL::Terminate() {
    TerminateContext();
}

void OpenGL::Resize(const window::Resolution &res) {

}

#ifdef WIN32

void OpenGL::CreateContext() {
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

void OpenGL::SwapBuffer() {
    SwapBuffers(window_.hdc);
}

void OpenGL::TerminateContext() {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(window_.hglrc);
    ReleaseDC(window_.hwnd, window_.hdc);
}

#else

void Dx12::CreateContext() {
    glfwMakeContextCurrent(window);
}

void Dx12::SwapBuffer() {
    glfwSwapBuffers(window);
}

void Dx12::TerminateContext() {

}

#endif

}
