/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file platform.hpp
 * @version 1.0
 * @date 09/03/2024
 * @brief Short description
 *
 * Platform especific stuff
 */

#pragma once

#ifdef WIN32

#include <DirectXMath.h>
#include <windows.h>

#define INLINE __forceinline

using namespace DirectX;
using WCallback = LRESULT(*)(HWND, UINT, WPARAM, LPARAM);

struct WHandle {
    HWND handle;
    HDC hdc;
    HGLRC hglrc;
};

#else
//#include <glm/glm.h>
#include "GLFW/glfw3.h"
#define INLINE inline


using WCallback = void(*)();
struct WHandle {
    GLFWwindow* handle;
};

#endif

