/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file input_types.hpp
 * @version 1.0
 * @date 02/11/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "common/common.hpp"
#include "math/math.hpp"

#include <unordered_map>
#include <utility>
#include <vector>
#include <functional>

namespace reveal3d::input {

enum class Code : u8 {
    MouseMove = 0x00U,
    MouseLeft = 0x01U,
    MouseRight = 0x02U,
    ControlCancel = 0x03U,
    MouseMiddle = 0x04U,
    MouseX1 = 0x05U,
    MouseX2 = 0x06U,
    KeyBackspace = 0x08U,
    KeyTab = 0x09U,
    KeySupr = 0x0CU,
    KeyEnter = 0x0DU,
    KeyShift = 0x10U,
    KeyControl = 0x11U,
    KeyMenu = 0x12U,
    KeyPause = 0x13U,
    KeyBlockShift = 0x14U,
    KeyEscape = 0x1BU,
    KeySpace = 0x20U,
    KeyPageUp = 0x21U,
    KeyPageDown = 0x22U,
    KeyEnd = 0x23U,
    KeyHome = 0x24U,
    KeyLeft = 0x25U,
    KeyUp = 0x26U,
    KeyRight = 0x27U,
    KeyDown = 0x28U,
    KeySelect = 0x29U,
    KeyPrint = 0x2AU,
    KeyExe = 0x2BU,
    KeyPrintScreen = 0x2CU,
    KeyInsert = 0x2CU,
    KeyDelete = 0x2EU,
    KeyHelp = 0x2FU,
    Key0 = 0x30U,
    Key1 = 0x31U,
    Key2 = 0x32U,
    Key3 = 0x33U,
    Key4 = 0x34U,
    Key5 = 0x35U,
    Key6 = 0x36U,
    Key7 = 0x37U,
    Key8 = 0x38U,
    Key9 = 0x39U,
    KeyA = 0x41U,
    KeyB = 0x42U,
    KeyC = 0x43U,
    KeyD = 0x44U,
    KeyE = 0x45U,
    KeyF = 0x46U,
    KeyG = 0x47U,
    KeyH = 0x48U,
    KeyI = 0x49U,
    KeyJ = 0x4AU,
    KeyK = 0x4BU,
    KeyL = 0x4CU,
    KeyM = 0x4DU,
    KeyN = 0x4EU,
    KeyO = 0x4FU,
    KeyP = 0x50U,
    KeyQ = 0x51U,
    KeyR = 0x52U,
    KeyS = 0x53U,
    KeyT = 0x54U,
    KeyU = 0x55U,
    KeyV = 0x56U,
    KeyW = 0x57U,
    KeyX = 0x58U,
    KeyY = 0x59U,
    KeyZ = 0x5AU,
    KeyLeftWin = 0x5BU,
    KeyRightWin = 0x5CU,
    KeyApps = 0x5DU,
    KeySleep = 0x5FU,
    KeyNumpad0 = 0x60U,
    KeyNumpad1 = 0x61U,
    KeyNumpad2 = 0x62U,
    KeyNumpad3 = 0x63U,
    KeyNumpad4 = 0x64U,
    KeyNumpad5 = 0x65U,
    KeyNumpad6 = 0x66U,
    KeyNumpad7 = 0x67U,
    KeyNumpad8 = 0x68U,
    KeyNumpad9 = 0x69U,
    KeyMultiply = 0x6AU,
    KeyAdd = 0x6BU,
    KeySeparator = 0x6CU,
    KeySubtract = 0x6DU,
    KeyDecimal = 0x6EU,
    KeyDivide = 0x6FU,
    KeyF1 = 0x70U,
    KeyF2 = 0x71U,
    KeyF3 = 0x72U,
    KeyF4 = 0x73U,
    KeyF5 = 0x74U,
    KeyF6 = 0x75U,
    KeyF7 = 0x76U,
    KeyF8 = 0x77U,
    KeyF9 = 0x78U,
    KeyF10 = 0x79U,
    KeyF11 = 0x7AU,
    KeyF12 = 0x7BU,
    KeyF13 = 0x7CU,
    KeyF14 = 0x7DU,
    KeyF15 = 0x7EU,
    KeyF16 = 0x7FU,
    KeyF17 = 0x80U,
    KeyF18 = 0x81U,
    KeyF19 = 0x82U,
    KeyF20 = 0x83U,
    KeyF21 = 0x84U,
    KeyF22 = 0x85U,
    KeyF23 = 0x86U,
    KeyF24 = 0x87U,
    KeyNumlock = 0x90U,
    KeyScrollock = 0x91U,
    KeyLeftShift = 0xA0U,
    KeyRightShift = 0xA1U,
    KeyLeftControl = 0xA2U,
    KeyRightControl = 0xA3U,
    KeyLeftAlt = 0xA4U,
    KeyRightAlt = 0xA5U,
    KeyColon = 0xBAU,
    KeyPlus = 0xBBU,
    KeyComma = 0xBCU,
    KeyMinus = 0xBDU,
    KeyPeriod = 0xBEU,

    invalid = 0xFFU
};

enum class Action : u8 {
    CameraFwd = 0,
    CameraBackwd,
    CameraUp,
    CameraDown,
    CameraLeft,
    CameraRight,
    CameraLook,
    WindowClose,
    ScenePause,
    SceneSelect,

    count,
    not_asigned
};

enum type : u8 {
    up = 0,
    down,
};

struct Binding {
    std::function<void(input::Action, input::type)> callback;
    std::function<void(input::Action, math::vec2)> mouse_callback;
};

struct Cursor {
    inline static math::vec2 pos {};
    inline static bool should_clip { false };
    inline static math::vec2 last_uncliped_pos {};
    //    static bool isLooking;
    //    static bool fistMouse;
};

}