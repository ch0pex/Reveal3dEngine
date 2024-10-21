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
#include "window/window_info.hpp"

#include "render/camera.hpp"

namespace reveal3d::graphics {

class Vulkan {
public:
    Vulkan(const window::Resolution &res) :width_(res.width), height_(res.height) {}
    void loadPipeline() {}
    void loadAssets() {}
    void update(render::Camera &camera) {}
    void setWindow(WHandle winHandle) {}

    [[nodiscard]] inline u32 getWidth() const { return width_; }
    [[nodiscard]] inline u32 getHeight() const { return height_; }


private:
    u32 width_;
    u32 height_;

};

}
