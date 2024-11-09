/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file platform.hpp
 * @version 1.0
 * @date 09/03/2024
 * @brief Platform
 *
 * Platform especific stuff
 */

#pragma once


#ifdef WIN32

#define NOMINMAX

#include <DirectXMath.h>
#include <windows.h>


// #define inline __forceinline
using namespace DirectX;

struct WHandle {
  HWND hwnd;
  HDC hdc;
  HGLRC hglrc;
};

#define GLFW_EXPOSE_NATIVE_WIN32

#else
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "GL/glew.h"

using WHandle = GLFWwindow*;

#endif
