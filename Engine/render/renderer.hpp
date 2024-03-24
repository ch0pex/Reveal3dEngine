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
#include "camera.hpp"

namespace reveal3d::render {

template<graphics::HRI Gfx>
class Renderer {
public:
    Renderer(const window::Resolution &res);
    void Init(WHandle wHandle);
    void Update();
    void Render();
    void Destroy();
    void Resize(const window::Resolution &res);
    [[nodiscard]] INLINE u32 GetWidth() const { return graphics_.GetWidth(); }
    [[nodiscard]] INLINE u32 GetHeight() const { return graphics_.GetHeight(); }
    //~Renderer();
private:
    void HandleInput();

    Timer timer_;
    Gfx graphics_;
    Camera camera_;
};

}