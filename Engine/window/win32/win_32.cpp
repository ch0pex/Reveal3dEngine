/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details) 
 ************************************************************************/
/**
 * @file win_32.cpp
 * @version 1.0
 * @date 27/02/2024
 * @brief Short description
 *
 * Longer description
 */

#include "win_32.hpp"
#include "window/window.hpp"
#include "input/input.hpp"


namespace reveal3d::window {


template<typename Gfx>
void Win32<Gfx>::Create(Renderer<Gfx> &renderer) {
    WNDCLASSEX windowClass = {
            .cbSize = sizeof(WNDCLASSEX),
            .style = CS_HREDRAW | CS_VREDRAW,
            .lpfnWndProc = info_.callback,
            .hInstance = GetModuleHandle(NULL),
            .hCursor = LoadCursor(NULL, IDC_ARROW),
            .lpszClassName = L"Reveal3dClass"
    };
    RegisterClassEx(&windowClass);

    RECT windowRect = {0, 0, static_cast<LONG>(info_.res.width), static_cast<LONG>(info_.res.height)};
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    info_.windowHandle = CreateWindow(
            windowClass.lpszClassName,
            info_.name,
            WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
            windowRect.right - windowRect.left,
            windowRect.bottom - windowRect.top,
            nullptr,
            nullptr,
            GetModuleHandle(NULL),
            &renderer);
}

/*
template<typename Gfx>
i32 Win32<Gfx>::Run(Renderer<Gfx> &renderer) {
    try {
        MSG msg = {};
        bool isRunning = true;

        ShowWindow(info_.windowHandle, SW_SHOW);


        while(isRunning) {
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                ClipMouse(renderer);
                TranslateMessage(&msg);
                DispatchMessage(&msg);
                isRunning &= (msg.message != WM_QUIT);
            }
        }
        renderer.Destroy();
        return static_cast<char>(msg.wParam);
    } catch(std::exception &e) {
        renderer.Destroy();
        log(logERROR) << e.what();
        MessageBoxA(info_.windowHandle, e.what(), NULL, MB_ICONERROR | MB_SETFOREGROUND);
        return EXIT_FAILURE;
    }
}
*/


template<typename Gfx>
void Win32<Gfx>::CloseWindow(input::action act, input::type type) {
    PostMessage(GetHwnd(), WM_CLOSE, 0, 0);
}

template<typename Gfx>
void Win32<Gfx>::ClipMouse(Renderer<Gfx> &renderer) {
    if (!input::cursor::shouldClip) return;

    if (input::cursor::pos.x < 2) {
        input::cursor::pos.x = info_.res.width - 3;
        renderer.CameraResetMouse();
    }
    else if (input::cursor::pos.x >= info_.res.width - 2) {
        input::cursor::pos.x = 3;
        renderer.CameraResetMouse();
    }
    if (input::cursor::pos.y < 2) {
        input::cursor::pos.y = info_.res.height - 3;
        renderer.CameraResetMouse();
    }
    else if (input::cursor::pos.y >= info_.res.height - 2) {
        input::cursor::pos.y = 3;
        renderer.CameraResetMouse();
    }

    POINT pt = {static_cast<LONG>(input::cursor::pos.x), static_cast<LONG>(input::cursor::pos.y)};
    ClientToScreen(info_.windowHandle, &pt);
    SetCursorPos(pt.x, pt.y);
}
template class Win32<graphics::dx::Graphics>;
//template class Win32<graphics::Vk::Graphics>;

}


