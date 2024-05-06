/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file glfwOpenGL.cpp
 * @version 1.0
 * @date 07/04/2024
 * @brief Short description
 *
 * Longer description
 */


#include "core/scene.hpp"
#include "render/viewport.hpp"
#include "math/math.hpp"

#include "Samples/common/scripts.hpp"

using namespace reveal3d;
using namespace reveal3d::graphics;


LogLevel loglevel = logDEBUG;


int  main() {

    Timer timer;
    f32 time = timer.TotalTime();
    window::InitInfo windowInitInfo(L"Reveal3d", 1920, 1080);
    Viewport<dx::Graphics, window::Win32> viewport(windowInitInfo);
    render::Viewport<opengl::Graphics, window::Glfw> viewport(windowInitInfo);

    core::Entity human = core::scene.AddEntityFromObj(relative(L"Assets/human.obj").c_str());
    core::Entity room = core::scene.AddEntityFromObj(relative(L"Assets/habitacion.obj").c_str());
    core::Scene *myScene = &core::scene;

//        MovementScript movementScript;
    for (u32 i = 0; i < 10; ++i)
        for (u32 j = 0; j < 10; ++j) {
            for (u32 k = 0; k < 20; ++k) {
                core::EntityInfo info = {
                        core::Transform({i * 1.5f, j * 1.5f, 1.5f * k}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}),
                        human.GetGeometry()};
                core::Entity entity = core::scene.AddEntity(info);
                entity.AddScript<HumanScript>();
            }
        }

    try {
        viewport.window_.Create(viewport.renderer_);
        viewport.renderer_.Init(viewport.window_.GetHandle());
        viewport.window_.Show();

        log(logDEBUG) << "Total Init time: " << timer.Diff(time);

        viewport.timer_.Reset();
        while(!viewport.window_.ShouldClose()) {
            viewport.timer_.Tick();
            viewport.window_.ClipMouse(viewport.renderer_);
            viewport.renderer_.Update();
            core::scene.Update(viewport.timer_.DeltaTime());
            viewport.window_.Update();
        }
        viewport.renderer_.Destroy();
    } catch(std::exception &e) {
        viewport.renderer_.Destroy();
        log(logERROR) << e.what();
        MessageBoxA(viewport.window_.GetHandle().hwnd, e.what(), NULL, MB_ICONERROR | MB_SETFOREGROUND);
        return EXIT_FAILURE;
    }

    return 0;
}
