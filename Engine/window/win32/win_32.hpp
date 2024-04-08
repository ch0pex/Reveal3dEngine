/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details) 
 ************************************************************************/
/**
 * @file win_32.hpp
 * @version 1.0
 * @date 27/02/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "render/renderer.hpp"
#include "window/window_info.hpp"
#include "input/input.hpp"
#include <WindowsX.h>

using namespace reveal3d::render;

namespace reveal3d::window {

template<typename Gfx>
class Win32 {
public:
    Win32(InitInfo &info);

    void Create(Renderer<Gfx> &renderer);
//    i32 Run(Renderer<Gfx> &renderer);
    void Show();
    void Update();
    void CloseWindow(input::action act, input::type type);
    void ClipMouse(Renderer<Gfx> &renderer);
    bool ShouldClose();

    [[nodiscard]] INLINE Resolution& GetRes() { return info_.res; }
    [[nodiscard]] INLINE WHandle GetHwnd() const { return info_.windowHandle; }
private:
    input::System<Win32> inputSystem_;
    InitInfo info_;
    MSG msg_ {};
    bool isRunning_ { false };
};

template<typename Gfx>
Win32<Gfx>::Win32(InitInfo &info) : info_(info)
{
   inputSystem_.AddHandlerDown(input::action::window_close, {&Win32::CloseWindow, nullptr, this});
}

template<typename Gfx>
void Win32<Gfx>::Create(Renderer<Gfx> &renderer) {
    WNDCLASSEXW windowClass = {
            .cbSize = sizeof(WNDCLASSEX),
            .style = CS_HREDRAW | CS_VREDRAW,
            .lpfnWndProc = info_.callback,
            .hInstance = GetModuleHandle(NULL),
            .hCursor = LoadCursor(NULL, IDC_ARROW),
            .lpszClassName = L"Reveal3dClass"
    };
    RegisterClassExW(&windowClass);

    RECT windowRect = {0, 0, static_cast<LONG>(info_.res.width), static_cast<LONG>(info_.res.height)};
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    info_.windowHandle = CreateWindowExW(
            0,
            windowClass.lpszClassName,
            info_.name,
            WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
            windowRect.right - windowRect.left,
            windowRect.bottom - windowRect.top,
            nullptr,
            nullptr,
            GetModuleHandle(nullptr),
            &renderer);
}

template<typename Gfx>
void Win32<Gfx>::Show() {
    ShowWindow(info_.windowHandle, SW_SHOW);
    isRunning_ = true;
}

template<typename Gfx>
bool Win32<Gfx>::ShouldClose() {
    PeekMessage(&msg_, NULL, 0, 0, PM_REMOVE);
    return !(isRunning_ );
}

template<typename Gfx>
void Win32<Gfx>::Update() {
    TranslateMessage(&msg_);
    DispatchMessage(&msg_);
    isRunning_ &= (msg_.message != WM_QUIT);
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
}