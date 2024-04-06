/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details) 
 ************************************************************************/
/**
 * @file renderer_.hpp
 * @version 1.0
 * @date 25/02/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "graphics/gfx.hpp"
#include "core/scene.hpp"
#include "camera.hpp"

namespace reveal3d::render {

template<graphics::HRI Gfx>
class Renderer {
public:
    Renderer(window::Resolution *res, Timer &timer);
    void Init(WHandle wHandle);
    void Update();
    void Render();
    void Destroy();
    void Resize(const window::Resolution &res);

    INLINE f32 DeltaTime() const { return timer_.DeltaTime(); }
    INLINE  void CameraResetMouse() { camera_.ResetMouse(); }

private:
    Gfx graphics_;
    Camera camera_;
    Timer& timer_;
};

}