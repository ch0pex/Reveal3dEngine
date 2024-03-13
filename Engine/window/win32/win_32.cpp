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


namespace reveal3d::window {


template<typename Gfx>
void Win32<Gfx>::InitWindow(Renderer<Gfx> &renderer) {
    WNDCLASSEX windowClass = {
            .cbSize = sizeof(WNDCLASSEX),
            .style = CS_HREDRAW | CS_VREDRAW,
            .lpfnWndProc = WindowProc,
            .hInstance = GetModuleHandle(NULL),
            .hCursor = LoadCursor(NULL, IDC_ARROW),
            .lpszClassName = L"Reveal3dClass"
    };
    RegisterClassEx(&windowClass);

    RECT windowRect = {0, 0, static_cast<LONG>(info_.res.width), static_cast<LONG>(info_.res.height)};
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    // Create the window_ and store a handle to it.
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

template<typename Gfx>
i32 Win32<Gfx>::Run(Renderer<Gfx> &renderer) {
    try {
        MSG msg = {};
        bool isRunning = true;

        InitWindow(renderer);
        ShowWindow(info_.windowHandle, SW_SHOW);

        renderer.Init(info_.windowHandle);
        while(isRunning) {
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
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

template<typename Gfx>
WHandle Win32<Gfx>::GetHwnd() const {
    return info_.windowHandle;
}

template<typename Gfx>
LRESULT Win32<Gfx>::WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

    auto* renderer = reinterpret_cast<Renderer<Gfx>*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (message) {
        case WM_CREATE:
        {
            LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
        }
            return 0;
        case WM_SIZE:
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT:
            try {
                renderer->Update();
                renderer->Render();
            } catch (std::exception &e) {
                log(logERROR) << e.what();
                renderer->Destroy();
                PostQuitMessage(0);
            }
            return 0;
        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE) {
                PostMessage(hwnd, WM_CLOSE, 0, 0);
                return 0;
            }
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

template class Win32<graphics::dx::Graphics>;
//template class Win32<graphics::Vk::Graphics>;

}


