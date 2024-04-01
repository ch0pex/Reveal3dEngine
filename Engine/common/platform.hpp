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

#ifdef _WIN32

#include <DirectXMath.h>
#include <windows.h>

#define INLINE __forceinline

using namespace DirectX;
using WHandle = HWND;
using WCallback = LRESULT(*)(WHandle, UINT, WPARAM, LPARAM);

#else
//#include <glm/glm.h>
#define INLINE inline

#endif

