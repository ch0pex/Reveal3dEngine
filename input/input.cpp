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
 * Longer descriptio    n
 */

#include "input.hpp"

namespace reveal3d::input  {

class System {
public:
    void AddHandlerUp(action action, Binding& binding) {
        handlersUp[action] = std::move(binding);
    }

    void AddHandlerDown(action action, Binding& binding) {
        handlersDown[action] = std::move(binding);
    }

    void AddMouseHandler(action action, Binding& binding) {
        mouseHandler[action] = std::move(binding);
    }

    void OnEventUp(action act, input::type type) {
        if (handlersUp.find(act) != handlersUp.end()) {
            handlersUp.at(act).callback(act, type);
        }
    }

    void OnEventDown(action act, input::type type) {
        if (handlersDown.find(act) != handlersDown.end()) {
            handlersDown.at(act).callback(act, type);
        }
    }

    void OnMouseDown(action act, math::vec2 value) {
        if (handlersDown.find(act) != handlersDown.end()) {
            handlersDown.at(act).mouse_callback(act, value);
        }
    }

    void OnMouseUp(action act, math::vec2 value) {
        if (handlersUp.find(act) != handlersUp.end()) {
            handlersUp.at(act).mouse_callback(act, value);
        }
    }

    void OnMouseMove(action act, math::vec2 value) {
        if (mouseHandler.find(act) != mouseHandler.end()) {
            mouseHandler.at(act).mouse_callback(act, value);
        }
    }

private:
    std::unordered_map<action, Binding> handlersDown;
    std::unordered_map<action, Binding> handlersUp;
    std::unordered_map<action, Binding> mouseHandler;
};

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

bool Cursor::shouldClip = false;
math::vec2 Cursor::pos = {};
math::vec2 Cursor::lastUnclipedPos = {};
System inputSystem;

void AddHandlerUp(action action, Binding&& binding) {
    inputSystem.AddHandlerUp(action, binding);
}

void AddHandlerDown(action action, Binding&& binding) {
    inputSystem.AddHandlerDown(action, binding);
}

void AddMouseHandler(action action, Binding&& binding) {
    inputSystem.AddMouseHandler(action, binding);
}

void KeyDown(u8 keycode) {
    if (bindings.find(keycode) != bindings.end()) {
        const action act = bindings[keycode];
        inputSystem.OnEventDown(act, type::down);
    }
}

void KeyDown(u8 keycode, math::vec2 pos) {
    if (bindings.find(keycode) != bindings.end()) {
        const action act = bindings[keycode];
        inputSystem.OnMouseDown(act, pos);
    }
}

void KeyUp(u8 keycode) {
    if (bindings.find(keycode) != bindings.end()) {
        const action act = bindings[keycode];
        inputSystem.OnEventUp(act, type::up);
    }
}

void KeyUp(u8 keycode, math::vec2 pos) {
    if (bindings.find(keycode) != bindings.end()) {
        const action act = bindings[keycode];
        inputSystem.OnEventDown(act, type::down);
    }
}

void MouseMove(u8 keycode, math::vec2 pos) {
    if (bindings.find(keycode) != bindings.end()) {
        const action act = bindings[keycode];
        inputSystem.OnMouseMove(act, pos);
    }
}

void BindKey(code keycode, action action) {
   bindings[keycode] = action;
}

void Unbind(code keycode) {
   bindings.erase(keycode);
}

}