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

class Win32 {
public:
    Win32(InitInfo &info);

    template<graphics::HRI Gfx> void Create(Renderer<Gfx> &renderer);
//    i32 Run(Renderer<Gfx> &renderer);
    void Show();
    void Update();
    void CloseWindow(input::action act, input::type type);
    template<graphics::HRI Gfx> void ClipMouse(Renderer<Gfx> &renderer);
    bool ShouldClose();

    [[nodiscard]] INLINE Resolution& GetRes() { return info_.res; }
    [[nodiscard]] INLINE WHandle GetHwnd() const { return info_.windowHandle; }
private:
    template<graphics::HRI Gfx> static LRESULT DefaultProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

    input::System<Win32> inputSystem_;
    InitInfo info_;
    MSG msg_ {};
    bool isRunning_ { false };
};

template<graphics::HRI Gfx>
LRESULT Win32::DefaultProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

    auto* renderer = reinterpret_cast<Renderer<Gfx>*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

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

Win32::Win32(InitInfo &info) : info_(info)
{
   inputSystem_.AddHandlerDown(input::action::window_close, {&Win32::CloseWindow, nullptr, this});
}

template<graphics::HRI Gfx>
void Win32::Create(Renderer<Gfx> &renderer) {
    WNDCLASSEXW windowClass = {
            .cbSize = sizeof(WNDCLASSEX),
            .style = CS_HREDRAW | CS_VREDRAW,
            .lpfnWndProc = info_.callback ? info_.callback : DefaultProc<Gfx>,
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

void Win32::Show() {
    ShowWindow(info_.windowHandle, SW_SHOW);
    isRunning_ = true;
}

bool Win32::ShouldClose() {
    PeekMessage(&msg_, NULL, 0, 0, PM_REMOVE);
    return !(isRunning_ );
}

void Win32::Update() {
    TranslateMessage(&msg_);
    DispatchMessage(&msg_);
    isRunning_ &= (msg_.message != WM_QUIT);
}

void Win32::CloseWindow(input::action act, input::type type) {
    PostMessage(GetHwnd(), WM_CLOSE, 0, 0);
}

template<graphics::HRI Gfx>
void Win32::ClipMouse(Renderer<Gfx> &renderer) {
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