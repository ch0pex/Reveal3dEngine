/************************************************************************
* Copyright (c) 2024 Alvaro Cabrera Barrio
* This code is licensed under MIT license (see LICENSE.txt for details)
************************************************************************/
/**
* @file vk_graphics.hpp
* @version 1.0
* @date 27/02/2024
* @brief Short description
*
* Longer description
*/

#pragma once

#include "common/common.hpp"

namespace reveal3d::graphics::Vk {

class Graphics {
public:
    Graphics(const window::Resolution &res) :width_(res.width), height_(res.height) {}
    void LoadPipeline() {}
    void LoadAssets() {}
    void Update(render::Camera &camera) {}
    void SetWindow(WHandle winHandle) {}

    [[nodiscard]] INLINE u32 GetWidth() const { return width_; }
    [[nodiscard]] INLINE u32 GetHeight() const { return height_; }


private:
    u32 width_;
    u32 height_;

};

}
