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

        window::InitInfo windowInitInfo(L"Reveal3d", 1920, 1080, &WindowProc);
        Viewport<dx::Graphics, window::Win32> viewport(windowInitInfo);
        //        Viewport<opengl::Graphics, window::Win32> viewport(windowInitInfo);

        //        Viewport<opengl::Graphics, window::Glfw> viewport(windowInitInfo);
        //        Viewport<dx::Graphics, window::Glfw> viewport(windowInitInfo);




        core::Entity human = core::scene.AddEntityFromObj(L"D:\\Universidad\\tfg\\engine\\Reveal3d\\Assets\\human.obj");
        core::Entity room = core::scene.AddEntityFromObj(L"D:\\Universidad\\tfg\\engine\\Reveal3d\\Assets\\habitacion.obj");
        //        core::Entity cube = core::scene.AddPrimitive(core::Geometry::primitive::cube);

        //        RotationScript rotationScript;
        //        MovementScript movementScript;

        //        human.AddScript(rotationScript);
        //        human.AddScript(movementScript);
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


LRESULT WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

    auto* renderer = reinterpret_cast<Renderer<dx::Graphics>*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (message) {
        case WM_CREATE:
        {
            LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
            return 0;
        }
        case WM_ENTERSIZEMOVE:
        case WM_EXITSIZEMOVE:
        case WM_SIZE:
        {
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            const window::Resolution res(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
            renderer->Resize(res);
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT:
            try {
                renderer->Render();
            } catch (std::exception &e) {
                log(logERROR) << e.what();
                renderer->Destroy();
                PostQuitMessage(0);
            }
            return 0;
        case WM_KEYDOWN:
            input::KeyDown(wParam);
            return 0;
        case WM_KEYUP:
            input::KeyUp(wParam);
            return 0;
        case WM_MBUTTONDOWN:
        {
            input::cursor::shouldClip = true;
            SetCapture(hwnd);
            input::KeyDown(input::code::mouse_middle);
            return 0;
        }
        case WM_MBUTTONUP:
        {
            input::cursor::shouldClip = false;
            input::KeyUp(input::code::mouse_middle);
            ReleaseCapture();
            return 0;
        }
        case WM_MOUSEMOVE:
        {
            input::cursor::pos = {(f32)GET_X_LPARAM(lParam), (f32)GET_Y_LPARAM(lParam)};
            input::MouseMove(wParam, input::cursor::pos);
            return 0;
        }
        case WM_RBUTTONDOWN:
            input::KeyDown(input::code::mouse_right, {(f32)GET_X_LPARAM(lParam), (f32)GET_Y_LPARAM(lParam)});
            return 0;
        case WM_RBUTTONUP:
            input::KeyUp(input::code::mouse_right, {(f32)GET_X_LPARAM(lParam), (f32)GET_Y_LPARAM(lParam)});
            return 0;
        case WM_LBUTTONDOWN:
            input::KeyDown(input::code::mouse_left, {(f32)GET_X_LPARAM(lParam), (f32)GET_Y_LPARAM(lParam)});
            return 0;
        case WM_LBUTTONUP:
            input::KeyUp(input::code::mouse_left, {(f32)GET_X_LPARAM(lParam), (f32)GET_Y_LPARAM(lParam)});
            return 0;

    }
    return DefWindowProcW(hwnd, message, wParam, lParam);
}

#else

int main() {
    Viewport<VkGraphics, Glfw> viewport;
}

#endif
