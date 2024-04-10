/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file win32OpenGL.cpp
 * @version 1.0
 * @date 07/04/2024
 * @brief Short description
 *
 * Longer description
 */


#include "core/scene.hpp"
#include "render/viewport.hpp"
#include "window/window.hpp"
#include "math/math.hpp"
#include "input/input.hpp"

#include "Samples/common/scripts.hpp"
#include "core/entity.hpp"
#include "graphics/gfx.hpp"

using namespace reveal3d;
using namespace reveal3d::graphics;


LogLevel loglevel = logDEBUG;


_Use_decl_annotations_
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    {

        Timer timer;
        f32 time = timer.TotalTime();
        window::InitInfo windowInitInfo(L"Reveal3d", 1920, 1080);
        Viewport<opengl::Graphics, window::Win32> viewport(windowInitInfo);

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
            viewport.window.Create(viewport.renderer);
            viewport.renderer.Init(viewport.window.GetHwnd());
            viewport.window.Show();

            viewport.timer_.Reset();
            while(!viewport.window.ShouldClose()) {
                viewport.timer_.Tick();
                viewport.window.ClipMouse(viewport.renderer);
                viewport.renderer.Update();
                core::scene.Update(viewport.timer_.DeltaTime());
                viewport.window.Update();
            }
            viewport.renderer.Destroy();
        } catch(std::exception &e) {
            viewport.renderer.Destroy();
            log(logERROR) << e.what();
            MessageBoxA(viewport.window.GetHwnd(), e.what(), NULL, MB_ICONERROR | MB_SETFOREGROUND);
            return EXIT_FAILURE;
        }
    }
#ifdef _DEBUG
    graphics::dx::utl::ReportLiveDeviceObjs();
#endif
    return 0;
}





