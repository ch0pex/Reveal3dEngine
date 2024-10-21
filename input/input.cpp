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
    void addHandlerUp(Action action, Binding &binding) { handlers_up_[action] = std::move(binding);
    }

    void addHandlerDown(Action action, Binding &binding) { handlers_down_[action] = std::move(binding);
    }

    void addMouseHandler(Action action, Binding &binding) { mouse_handler_[action] = std::move(binding);
    }

    void onEventUp(Action act, input::type type) {
        if (handlers_up_.find(act) != handlers_up_.end()) {
            handlers_up_.at(act).callback(act, type);
        }
    }

    void onEventDown(Action act, input::type type) {
        if (handlers_down_.find(act) != handlers_down_.end()) {
            handlers_down_.at(act).callback(act, type);
        }
    }

    void onMouseDown(Action act, math::vec2 value) {
        if (handlers_down_.find(act) != handlers_down_.end()) {
            handlers_down_.at(act).mouse_callback(act, value);
        }
    }

    void onMouseUp(Action act, math::vec2 value) {
        if (handlers_up_.find(act) != handlers_up_.end()) {
            handlers_up_.at(act).mouse_callback(act, value);
        }
    }

    void onMouseMove(Action act, math::vec2 value) {
        if (mouse_handler_.find(act) != mouse_handler_.end()) {
            mouse_handler_.at(act).mouse_callback(act, value);
        }
    }

private:
    std::unordered_map<Action, Binding> handlers_down_;
    std::unordered_map<Action, Binding> handlers_up_;
    std::unordered_map<Action, Binding> mouse_handler_;
};

std::unordered_map<u8, Action> bindings = {
        {Code::KeyEscape, Action::WindowClose},
        {Code::MouseMiddle, Action::CameraLook},
        {Code::MouseMove, Action::CameraLook},
        {Code::KeyShift, Action::CameraLook},
        {Code::KeyS, Action::CameraBackwd},
        {Code::KeyW, Action::CameraFwd},
        {Code::KeyA, Action::CameraLeft},
        {Code::KeyD, Action::CameraRight},
        {Code::KeyE, Action::CameraUp},
        {Code::KeyQ, Action::CameraDown},
        {Code::KeyP, Action::ScenePause}
};

bool Cursor::should_clip = false;
math::vec2 Cursor::pos = {};
math::vec2 Cursor::last_uncliped_pos = {};
System inputSystem;

void add_handler_up(Action action, Binding &&binding) { inputSystem.addHandlerUp(action, binding); }

void add_handler_down(Action action, Binding &&binding) { inputSystem.addHandlerDown(action, binding); }

void add_mouse_handler(Action action, Binding &&binding) { inputSystem.addMouseHandler(action, binding); }

void key_down(u8 keycode) {
    if (bindings.find(keycode) != bindings.end()) {
        const Action act = bindings[keycode];
        inputSystem.onEventDown(act, type::down);
    }
}

void key_down(u8 keycode, math::vec2 pos) {
    if (bindings.find(keycode) != bindings.end()) {
        const Action act = bindings[keycode];
        inputSystem.onMouseDown(act, pos);
    }
}

void key_up(u8 keycode) {
    if (bindings.find(keycode) != bindings.end()) {
        const Action act = bindings[keycode];
        inputSystem.onEventUp(act, type::up);
    }
}

void key_up(u8 keycode, math::vec2 pos) {
    if (bindings.find(keycode) != bindings.end()) {
        const Action act = bindings[keycode];
        inputSystem.onEventDown(act, type::down);
    }
}

void mouse_move(u8 keycode, math::vec2 pos) {
    if (bindings.find(keycode) != bindings.end()) {
        const Action act = bindings[keycode];
        inputSystem.onMouseMove(act, pos);
    }
}

void bind_key(Code keycode, Action action) {
   bindings[keycode] = action;
}

void unbind(Code keycode) {
   bindings.erase(keycode);
}

}