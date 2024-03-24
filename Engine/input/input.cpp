/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file input.cpp
 * @version 1.0
 * @date 22/03/2024
 * @brief Short description
 *
 * Longer description
 */

#include "input.hpp"


namespace reveal3d::input  {

std::vector<BaseSystem *> inputSystems;
std::unordered_map<u8, action> bindings = {
        {code::key_escape, action::window_close},
        {code::mouse_middle, action::camera_look},
        {code::mouse_move, action::camera_look},
        {code::key_shift, action::camera_look},
        {code::key_s, action::camera_backwd},
        {code::key_w, action::camera_fwd},
        {code::key_a, action::camera_left},
        {code::key_d, action::camera_right},
        {code::key_e, action::camera_up},
        {code::key_q, action::camera_down},
        {code::key_p, action::scene_pause}
};

void KeyDown(u8 keycode) {
    if (bindings.find(keycode) != bindings.end()) {
        const action act = bindings[keycode];
        for(auto *inputSys : inputSystems) {
            inputSys->OnEventDown(act, type::down);
        }
    }
}

void KeyDown(u8 keycode, math::vec2 pos) {
    if (bindings.find(keycode) != bindings.end()) {
        const action act = bindings[keycode];
        for(auto *inputSys : inputSystems) {
            inputSys->OnMouseDown(act, pos);
        }
    }

}

void KeyUp(u8 keycode) {
    if (bindings.find(keycode) != bindings.end()) {
        const action act = bindings[keycode];
        for (auto *inputSys: inputSystems) {
            inputSys->OnEventUp(act, type::up);
        }
    }
}

void KeyUp(u8 keycode, math::vec2 pos) {
    if (bindings.find(keycode) != bindings.end()) {
        const action act = bindings[keycode];
        for (auto *inputSys: inputSystems) {
            inputSys->OnMouseUp(act, pos);
        }
    }
}

void MouseMove(u8 keycode, math::vec2 pos) {
    if (bindings.find(keycode) != bindings.end()) {
        const action act = bindings[keycode];
        for (auto *inputSys: inputSystems) {
            inputSys->OnMouseMove(act, pos);
        }
    }
}

void BindKey(code keycode, action action) {
   bindings[keycode] = action;
}

void Unbind(code keycode) {
   bindings.erase(keycode);
}

BaseSystem::BaseSystem() {
    inputSystems.emplace_back(this);
}

BaseSystem::~BaseSystem() {
    auto it = std::find(inputSystems.begin(), inputSystems.end(), this);
    inputSystems.erase(it);
}

}