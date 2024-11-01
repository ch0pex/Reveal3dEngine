/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file window_info.hpp
 * @version 1.0
 * @date 09/03/2024
 * @brief Window info
 *
 * Window initialization info
 */

#pragma once

#include "common/common.hpp"

namespace reveal3d::window {

using WCallback = uint8_t *;

struct Resolution {
    Resolution(u32 width, u32 height) :
        width(width), height(height) , aspect_ratio(static_cast<f32>(width) / static_cast<f32>(height)) {}
    u32 width;
    u32 height;
    f32 aspect_ratio;
};


struct Info {
    Info() = default;
    Info(const wchar_t * name, u32 width, u32 height) : name(name), res(width, height) {}
    Info(const wchar_t * name, u32 width, u32 height, WCallback callback) :
        name(name), res(width, height), callback(callback) {}
    const wchar_t * name { L"Reveal3d" };
    Resolution res { 1920, 1080 };
    WHandle handle { nullptr };
    WCallback callback { nullptr };
};

}
