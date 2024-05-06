/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file window_info.hpp
 * @version 1.0
 * @date 09/03/2024
 * @brief Short description
 *
 * Window initialization info
 */

#pragma once

#include "common/common.hpp"

namespace reveal3d::window {

using WCallback = uint8_t *;

struct Resolution {
    Resolution(u32 width, u32 height) :
        width(width), height(height) ,
        aspectRatio(static_cast<f32>(width) / static_cast<f32>(height)) {}
    u32 width;
    u32 height;
    f32 aspectRatio;
};


struct InitInfo {
    InitInfo() = default;
    InitInfo(const wchar_t * name, u32 width, u32 height) : name(name), res(width, height) {}
    InitInfo(const wchar_t * name, u32 width, u32 height, WCallback callback) :
        name(name), res(width, height), callback(callback) {}
    const wchar_t * name { L"Reveal3d" };
    Resolution res { 1920, 1080 };
    WHandle handle { nullptr };
    WCallback callback { nullptr };
};

}
