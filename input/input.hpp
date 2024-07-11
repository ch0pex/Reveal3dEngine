/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file input.hpp
 * @version 1.0
 * @date 07/03/2024
 * @brief Short description
 *
 * Input module has two levels of abstraction above callbacks
 *
 * -------------------------------------
 * |   Code  |  Action   |  Callback   |
 * -------------------------------------
 *
 */

#pragma once

#include "common/common.hpp"
#include "math/math.hpp"
#include <unordered_map>
#include <vector>

namespace reveal3d::input {

enum code : u8 {
    mouse_move = 0x00U,
    mouse_left = 0x01U,
    mouse_right = 0x02U,
    control_cancel = 0x03U,
    mouse_middle = 0x04U,
    mouse_x1 = 0x05U,
    mouse_x2 = 0x06U,
    key_backspace = 0x08U,
    key_tab = 0x09U,
    key_supr = 0x0CU,
    key_enter = 0x0DU,
    key_shift = 0x10U,
    key_control = 0x11U,
    key_menu = 0x12U,
    key_pause = 0x13U,
    key_block_shift = 0x14U,
    key_escape = 0x1BU,
    key_space = 0x20U,
    key_page_up = 0x21U,
    key_page_down = 0x22U,
    key_end = 0x23U,
    key_home = 0x24U,
    key_left = 0x25U,
    key_up = 0x26U,
    key_right = 0x27U,
    key_down = 0x28U,
    key_select = 0x29U,
    key_print = 0x2AU,
    key_exe = 0x2BU,
    key_print_screen = 0x2CU,
    key_insert = 0x2CU,
    key_delete = 0x2EU,
    key_help = 0x2FU,
    key_0 = 0x30U,
    key_1 = 0x31U,
    key_2 = 0x32U,
    key_3 = 0x33U,
    key_4 = 0x34U,
    key_5 = 0x35U,
    key_6 = 0x36U,
    key_7 = 0x37U,
    key_8 = 0x38U,
    key_9 = 0x39U,
    key_a = 0x41U,
    key_b = 0x42U,
    key_c = 0x43U,
    key_d = 0x44U,
    key_e = 0x45U,
    key_f = 0x46U,
    key_g = 0x47U,
    key_h = 0x48U,
    key_i = 0x49U,
    key_j = 0x4AU,
    key_k = 0x4BU,
    key_l = 0x4CU,
    key_m = 0x4DU,
    key_n = 0x4EU,
    key_o = 0x4FU,
    key_p = 0x50U,
    key_q = 0x51U,
    key_r = 0x52U,
    key_s = 0x53U,
    key_t = 0x54U,
    key_u = 0x55U,
    key_v = 0x56U,
    key_w = 0x57U,
    key_x = 0x58U,
    key_y = 0x59U,
    key_z = 0x5AU,
    key_left_win = 0x5BU,
    key_right_win = 0x5CU,
    key_apps = 0x5DU,
    key_sleep = 0x5FU,
    key_numpad_0 = 0x60U,
    key_numpad_1 = 0x61U,
    key_numpad_2 = 0x62U,
    key_numpad_3 = 0x63U,
    key_numpad_4 = 0x64U,
    key_numpad_5 = 0x65U,
    key_numpad_6 = 0x66U,
    key_numpad_7 = 0x67U,
    key_numpad_8 = 0x68U,
    key_numpad_9 = 0x69U,
    key_multiply = 0x6AU,
    key_add = 0x6BU,
    key_separator = 0x6CU,
    key_subtract = 0x6DU,
    key_decimal = 0x6EU,
    key_divide = 0x6FU,
    key_f1 = 0x70U,
    key_f2 = 0x71U,
    key_f3 = 0x72U,
    key_f4 = 0x73U,
    key_f5 = 0x74U,
    key_f6 = 0x75U,
    key_f7 = 0x76U,
    key_f8 = 0x77U,
    key_f9 = 0x78U,
    key_f10 = 0x79U,
    key_f11 = 0x7AU,
    key_f12 = 0x7BU,
    key_f13 = 0x7CU,
    key_f14 = 0x7DU,
    key_f15 = 0x7EU,
    key_f16 = 0x7FU,
    key_f17 = 0x80U,
    key_f18 = 0x81U,
    key_f19 = 0x82U,
    key_f20 = 0x83U,
    key_f21 = 0x84U,
    key_f22 = 0x85U,
    key_f23 = 0x86U,
    key_f24 = 0x87U,
    key_numlock = 0x90U,
    key_scrollock = 0x91U,
    key_left_shift = 0xA0U,
    key_right_shift = 0xA1U,
    key_left_control = 0xA2U,
    key_right_control = 0xA3U,
    key_left_alt = 0xA4U,
    key_right_alt = 0xA5U,
    key_colon = 0xBAU,
    key_plus = 0xBBU,
    key_comma = 0xBCU,
    key_minus = 0xBDU,
    key_period = 0xBEU,

    invalid = 0xFFU
};

enum action : u8 {
    camera_fwd = 0,
    camera_backwd,
    camera_up,
    camera_down,
    camera_left,
    camera_right,
    camera_look,
    window_close,
    scene_pause,
    scene_select,

    count,
    not_asigned
};

enum type {
    up = 0,
    down,
};

template<typename T>
struct Binding {
    void (T::*callback) (input::action, input::type);
    void (T::*mouseCallback) (input::action, math::vec2);
    T* instance;
};


void KeyDown(u8 keycode);
void KeyDown(u8 keycode, math::vec2 pos);
void KeyUp(u8 keycode);
void KeyUp(u8 keycode, math::vec2 pos);
void MouseMove(u8 keycode, math::vec2 pos);
void BindKey(code keycode, action action);
void Unbind(code keycode);

class BaseSystem {
public:
    virtual void OnEventDown(action act, input::type type) = 0;
    virtual void OnEventUp(action act, input::type type) = 0;
    virtual void OnMouseMove(action act, math::vec2 value) = 0;
    virtual void OnMouseDown(action act, math::vec2 value) = 0;
    virtual void OnMouseUp(action act, math::vec2 value) = 0;
protected:
    BaseSystem();
    ~BaseSystem();

};

template<typename T>
class System : BaseSystem {
public:
    void AddHandlerUp(action action, Binding<T> binding) {
        handlersUp[action] = binding;
    }

    void AddHandlerDown(action action, Binding<T> binding) {
        handlersDown[action] = binding;
    }

    void AddMouseHandler(action action, Binding<T> binding) {
       mouseHandler[action] = binding;
    }

    void OnEventUp(action act, input::type type) override {
        if (handlersUp.find(act) != handlersUp.end()) {
            (handlersUp[act].instance->*handlersUp[act].callback)(act, type);
        }
    }

    void OnEventDown(action act, input::type type) override {
        if (handlersDown.find(act) != handlersDown.end()) {
            (handlersDown[act].instance->*handlersDown[act].callback)(act, type);
        }
    }

    void OnMouseDown(action act, math::vec2 value) override {
        if (handlersDown.find(act) != handlersDown.end()) {
            (handlersDown[act].instance->*handlersDown[act].mouseCallback)(act, value);
        }
    }

    void OnMouseUp(action act, math::vec2 value) override {
        if (handlersUp.find(act) != handlersUp.end()) {
            (handlersUp[act].instance->*handlersUp[act].mouseCallback)(act, value);
        }
    }

    void OnMouseMove(action act, math::vec2 value) override{
        if (mouseHandler.find(act) != mouseHandler.end()) {
            (mouseHandler[act].instance->*mouseHandler[act].mouseCallback)(act, value);
        }
    }

private:
    std::unordered_map<action, Binding<T>> handlersDown;
    std::unordered_map<action, Binding<T>> handlersUp;
    std::unordered_map<action, Binding<T>> mouseHandler;
};

struct cursor {
    static math::vec2 pos;
    static bool shouldClip;
    static math::vec2 lastUnclipedPos;
    //    static bool isLooking;
    //    static bool fistMouse;
};
}