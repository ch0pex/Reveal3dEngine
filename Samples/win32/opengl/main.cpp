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
#include "graphics/directX12/dx_utils.hpp"

using namespace reveal3d;
using namespace reveal3d::graphics;


LogLevel loglevel = logDEBUG;

#if defined(_WIN32)

LRESULT WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);



_Use_decl_annotations_
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    {

        window::InitInfo windowInitInfo(L"Reveal3d", 1920, 1080);
        Viewport<dx::Graphics, window::Win32> viewport(windowInitInfo);
        //        Viewport<opengl::Graphics, window::Win32> viewport(windowInitInfo);

        //        Viewport<opengl::Graphics, window::Glfw> viewport(windowInitInfo);
        //        Viewport<dx::Graphics, window::Glfw> viewport(windowInitInfo);




        core::Entity human = core::scene.AddEntityFromObj(relative(L"Assets/human.obj").c_str());
        core::Entity room = core::scene.AddEntityFromObj(relative(L"Assets\\habitacion.obj").c_str());
        core::Entity cube = core::scene.AddPrimitive(core::Geometry::primitive::cube);

        RotationScript rS[10][10][10];
        RotationScript mS[10][10][10];
        for (u32 i = 0; i < 10; ++i)
        for (u32 j = 0; j < 10; ++j)
        for (u32 k = 0; k < 10; ++k){
            core::EntityInfo info = {
                core::Transform({i * 1.5f, j * 1.5f, k * 1.5f}, {1.0f, 1.0f, 1.0f}, {0.0f,0.0f,0.0f}),
                human.GetGeometry()
            };
            core::scene.AddEntity(info);
        }


        RotationScript rotationScript;
        MovementScript movementScript;

        human.AddScript(rotationScript);
        human.AddScript(movementScript);
        human.AddMesh(core::Geometry::cube);
        //        cube.AddScript(rotationScript);
        room.SetScale(3);
        room.SetPosition(0.0f, 5.0f,0.0f);

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




#else

int main() {
    Viewport<VkGraphics, Glfw> viewport;
}

#endif
