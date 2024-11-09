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

#include "input_types.hpp"

namespace reveal3d::input {

namespace detail {

class System {
public:
    void addHandlerUp(const Action action, Binding &&binding) { handlers_up_[action] = std::move(binding); }

    void addHandlerDown(const Action action, Binding &&binding) { handlers_down_[action] = std::move(binding); }

    void addMouseHandler(const Action action, Binding &&binding) { mouse_handler_[action] = std::move(binding); }

    void onEventUp(const Action act, const input::type type) const {
        if (handlers_up_.contains(act)  ) {
            handlers_up_.at(act).callback(act, type);
        }
    }

    void onEventDown(const Action act, const input::type type) const {
        if (handlers_down_.contains(act)  ) {
            handlers_down_.at(act).callback(act, type);
        }
    }

    void onMouseDown(const Action act, const math::vec2 value) const {
        if (handlers_down_.contains(act)  ) {
            handlers_down_.at(act).mouse_callback(act, value);
        }
    }

    void onMouseUp(const Action act, const math::vec2 value) const {
        if (handlers_up_.contains(act)  ) {
            handlers_up_.at(act).mouse_callback(act, value);
        }
    }

    void onMouseMove(const Action act, const math::vec2 value) const {
        if (mouse_handler_.contains(act)  ) {
            mouse_handler_.at(act).mouse_callback(act, value);
        }
    }

private:
    std::unordered_map<Action, Binding> handlers_down_;
    std::unordered_map<Action, Binding> handlers_up_;
    std::unordered_map<Action, Binding> mouse_handler_;
};

inline std::unordered_map<Code, Action> bindings = {
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

inline System inputSystem;

}



inline void add_handler_up(const Action action, Binding&& binding) {
    detail::inputSystem.addHandlerUp(action, std::move(binding));
}

inline void add_handler_down(const Action action, Binding&& binding) {
    detail::inputSystem.addHandlerDown(action, std::move(binding));
}

inline void add_mouse_handler(const Action action, Binding&& binding) {
    detail::inputSystem.addMouseHandler(action, std::move(binding));
}

inline void key_down(const Code keycode) {
    if (detail::bindings.contains(keycode)) {
        const Action act = detail::bindings[keycode];
        detail::inputSystem.onEventDown(act, type::down);
    }
}

inline void key_down(const Code keycode, const math::vec2 pos) {
    if (detail::bindings.contains(keycode)  ) {
        const Action act = detail::bindings[keycode];
        detail::inputSystem.onMouseDown(act, pos);
    }
}

inline void key_up(const Code keycode) {
    if (detail::bindings.contains(keycode)  ) {
        const Action act = detail::bindings[keycode];
        detail::inputSystem.onEventUp(act, type::up);
    }
}

inline void key_up(const Code keycode, math::vec2 pos) {
    if (detail::bindings.contains(keycode)  ) {
        const Action act = detail::bindings[keycode];
        detail::inputSystem.onEventDown(act, type::down);
    }
}

inline void mouse_move(const Code keycode, const math::vec2 pos) {
    if (detail::bindings.contains(keycode)  ) {
        const Action act = detail::bindings[keycode];
        detail::inputSystem.onMouseMove(act, pos);
    }
}

inline void bind_key(const Code keycode, const Action action) {
   detail::bindings[keycode] = action;
}

inline void unbind(const Code keycode) {
   detail::bindings.erase(keycode);
}

}