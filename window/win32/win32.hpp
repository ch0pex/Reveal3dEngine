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
 * Win32 backend manager class
 */

#pragma once

#include "input/input.hpp"
#include "render/renderer.hpp"
#include "window/window_info.hpp"

#include <WindowsX.h>
#include <backends/imgui_impl_win32.h>


namespace reveal3d::window {

class Win32 {
public:
    using WCallback = LRESULT (*)(HWND, UINT, WPARAM, LPARAM);

    explicit Win32(const Info& info);

    template<graphics::HRI Gfx>
    void create(render::Renderer<Gfx>& renderer);
    template<graphics::HRI Gfx>
    void update(render::Renderer<Gfx>& renderer);
    void show();
    void closeWindow(input::Action act, input::type type);
    bool shouldClose();

    [[nodiscard]] inline Resolution& getRes() { return info_.res; }
    [[nodiscard]] inline WHandle getHandle() const { return info_.handle; }

private:
    template<graphics::HRI Gfx>
    void clipMouse(render::Renderer<Gfx>& renderer);
    template<graphics::HRI Gfx>
    static LRESULT defaultProc(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param);

    Info info_;
    MSG msg_{};
    WCallback callback_{nullptr};
    bool is_running_{false};
};

template<graphics::HRI Gfx>
void Win32::update(render::Renderer<Gfx>& renderer) {
    while (PeekMessage(&msg_, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg_);
        DispatchMessage(&msg_);
        is_running_ &= (msg_.message != WM_QUIT);
    }
    clipMouse(renderer);
}

template<graphics::HRI Gfx>
LRESULT Win32::defaultProc(const HWND hwnd, const UINT message, const WPARAM w_param, const LPARAM l_param) {

#ifdef IMGUI
    extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    if (ImGui_ImplWin32_WndProcHandler(hwnd, message, w_param, l_param))
        return true;
#endif
    auto* renderer = reinterpret_cast<render::Renderer<Gfx>*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (message) {
        case WM_CREATE: {
            LPCREATESTRUCT p_create_struct = reinterpret_cast<LPCREATESTRUCT>(l_param);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(p_create_struct->lpCreateParams));
            return 0;
        }
        case WM_ENTERSIZEMOVE:
        case WM_EXITSIZEMOVE:
        case WM_SIZE: {
            RECT client_rect;
            GetClientRect(hwnd, &client_rect);
            const window::Resolution res(client_rect.right - client_rect.left, client_rect.bottom - client_rect.top);
            renderer->resize(res);
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_KEYDOWN: {
#ifdef IMGUI
            const ImGuiIO& io = ImGui::GetIO();
            (void)io;
            if (io.WantCaptureMouse)
                return 0;
#endif
            input::key_down(w_param);
            return 0;
        }
        case WM_KEYUP:
            input::key_up(w_param);
            return 0;
        case WM_MBUTTONDOWN: {
            input::Cursor::should_clip = true;
            input::Cursor::last_uncliped_pos = {(f32)GET_X_LPARAM(l_param), (f32)GET_Y_LPARAM(l_param)};
            SetCapture(hwnd);
            SetCursor(NULL);
            input::key_down(input::Code::MouseMiddle);
            return 0;
        }
        case WM_MBUTTONUP: {
            input::Cursor::should_clip = false;
            input::key_up(input::Code::MouseMiddle);
            RECT window_rect;
            GetWindowRect(hwnd, &window_rect);
            SetCursorPos(input::Cursor::last_uncliped_pos.x + window_rect.left,
                         input::Cursor::last_uncliped_pos.y + window_rect.top);
            ReleaseCapture();
            return 0;
        }
        case WM_MOUSEMOVE: {
            input::Cursor::pos = {static_cast<f32>(GET_X_LPARAM(l_param)), static_cast<f32>(GET_Y_LPARAM(l_param))};
            input::mouse_move(w_param, input::Cursor::pos);
            return 0;
        }
        case WM_RBUTTONDOWN:
            input::key_down(input::Code::MouseRight, {(f32)GET_X_LPARAM(l_param), (f32)GET_Y_LPARAM(l_param)});
            return 0;
        case WM_RBUTTONUP:
            input::key_up(input::Code::MouseRight, {(f32)GET_X_LPARAM(l_param), (f32)GET_Y_LPARAM(l_param)});
            return 0;
        case WM_LBUTTONDOWN:
            input::key_down(input::Code::MouseLeft, {(f32)GET_X_LPARAM(l_param), (f32)GET_Y_LPARAM(l_param)});
            return 0;
        case WM_LBUTTONUP:
            input::key_up(input::Code::MouseLeft, {(f32)GET_X_LPARAM(l_param), (f32)GET_Y_LPARAM(l_param)});
            return 0;
    }
    return DefWindowProcW(hwnd, message, w_param, l_param);
}

template<graphics::HRI Gfx>
void Win32::create(render::Renderer<Gfx>& renderer) {
    const WNDCLASSEXW window_class = {.cbSize = sizeof(WNDCLASSEX),
                                      .style = CS_HREDRAW | CS_VREDRAW,
                                      .lpfnWndProc = callback_ ? callback_ : defaultProc<Gfx>,
                                      .hInstance = GetModuleHandle(nullptr),
                                      .hCursor = LoadCursor(nullptr, IDC_ARROW),
                                      .lpszClassName = L"Reveal3dClass"};
    RegisterClassExW(&window_class);

    RECT window_rect = {0, 0, static_cast<LONG>(info_.res.width), static_cast<LONG>(info_.res.height)};
    AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, FALSE);

    info_.handle.hwnd =
            CreateWindowExW(0, window_class.lpszClassName, info_.name, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                            CW_USEDEFAULT, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top,
                            nullptr, nullptr, GetModuleHandle(nullptr), &renderer);
}

template<graphics::HRI Gfx>
void Win32::clipMouse(render::Renderer<Gfx>& renderer) {
    if (!input::Cursor::should_clip)
        return;

    if (input::Cursor::pos.x < 2) {
        input::Cursor::pos.x = info_.res.width - 3;
        renderer.cameraResetMouse();
    }
    else if (input::Cursor::pos.x >= info_.res.width - 2) {
        input::Cursor::pos.x = 3;
        renderer.cameraResetMouse();
    }
    if (input::Cursor::pos.y < 2) {
        input::Cursor::pos.y = info_.res.height - 3;
        renderer.cameraResetMouse();
    }
    else if (input::Cursor::pos.y >= info_.res.height - 2) {
        input::Cursor::pos.y = 3;
        renderer.cameraResetMouse();
    }

    POINT pt = {static_cast<LONG>(input::Cursor::pos.x), static_cast<LONG>(input::Cursor::pos.y)};
    ClientToScreen(info_.handle.hwnd, &pt);
    SetCursorPos(pt.x, pt.y);
}


} // namespace reveal3d::window
