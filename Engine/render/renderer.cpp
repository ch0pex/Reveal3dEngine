/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file renderer_.cpp
 * @version 1.0
 * @date 25/02/2024
 * @brief Short description
 *
 * Longer description
 */

#include "renderer.hpp"

namespace reveal3d::render {

template<graphics::HRI Gfx>
Renderer<Gfx>::Renderer(const window::Resolution &res) :
        camera_(res),
        graphics_(res)
{

}

template<graphics::HRI Gfx>
void Renderer<Gfx>::Init(WHandle wHandle) {

    graphics_.SetWindow(wHandle);
    graphics_.LoadPipeline();
    graphics_.LoadAssets();
}

template<graphics::HRI Gfx>
void Renderer<Gfx>::Update() {
    timer_.Tick();
    camera_.Update(timer_.DeltaTime());
    graphics_.Update(camera_);
}

template<graphics::HRI Gfx>
void Renderer<Gfx>::Render() {
    graphics_.PopulateCommands();
    graphics_.Draw();
    graphics_.MoveToNextFrame();
}

template<graphics::HRI Gfx>
void Renderer<Gfx>::Destroy() {
    log(logDEBUG) << "Terminating renderer...\n";
    graphics_.Terminate();
}

template<graphics::HRI Gfx>
void Renderer<Gfx>::Resize(u32 width, u32 height) {
    //camera.Resize();
    //graphics.Resize();
}

template class Renderer<graphics::Dx::Graphics>;

} // namespace reveal3d::render
