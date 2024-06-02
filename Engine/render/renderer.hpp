/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details) 
 ************************************************************************/
/**
 * @file renderer.hpp
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

#include "IMGUI/backends/imgui_impl_win32.h"
#include "IMGUI/backends/imgui_impl_dx12.h"

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

    Gfx& Graphics() { return graphics_; }

    INLINE f32 DeltaTime() const { return timer_.DeltaTime(); }
    INLINE  void CameraResetMouse() { camera_.ResetMouse(); }

private:
    Gfx graphics_;
    Camera camera_;
    Timer& timer_;
};

template<graphics::HRI Gfx>
Renderer<Gfx>::Renderer(window::Resolution *res, Timer &timer)
        : camera_(*res),
          graphics_(res),
          timer_(timer)
{

}

template<graphics::HRI Gfx>
void Renderer<Gfx>::Init(WHandle wHandle) {
    f32 time = timer_.TotalTime();
    graphics_.SetWindow(wHandle);
    graphics_.LoadPipeline();
    log(logDEBUG) << "Initializing Pipeline...[" << timer_.Diff(time) * 1000 <<"ms]";
    time = timer_.TotalTime();
    graphics_.LoadAssets();
    log(logDEBUG) << "Loading assets...[" << timer_.Diff(time) * 1000 <<"ms]";
}

template<graphics::HRI Gfx>
void Renderer<Gfx>::Update() {
    camera_.Update(timer_);
    graphics_.Update(camera_);
}



template<graphics::HRI Gfx>
void Renderer<Gfx>::Render() {
    static bool d_window = true;
    static bool an_window = false;
    static ImVec4 cl = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
//    ImGui::DockSpaceOverViewport();
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).

    if (d_window)
        ImGui::ShowDemoWindow(&d_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &d_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &an_window);

        core::Transform& t = core::scene.GetTransform(0);
//        t.SetPosition(t.Position() + math::vec3{4.0f, 0.0f, 0.0f});
        math::xvec3 pos = t.Position();
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::Text("Position");               // Display some text (you can use a format strings too)
        ImGui::SameLine();
        ImGui::DragFloat3("##vec3", (f32*)&pos, 0.01f); // Edit 3 floats representing a color
        t.SetPosition(pos);

        if (ImGui::Button("Button")) {
        }                           // Buttons return true when clicked (most widgets return true when edited/activated)
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", timer_.Fps());
        ImGui::End();
    }

    // 3. Show another simple window.
    if (an_window)
    {
        ImGui::Begin("Another Window", &an_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            an_window = false;
        ImGui::End();
    }

    // Rendering
    ImGui::Render();
    graphics_.PrepareRender();
    graphics_.Draw();
}

template<graphics::HRI Gfx>
void Renderer<Gfx>::Destroy() {
    log(logDEBUG) << "Cleaning pipeline...[" << timer_.TotalTime()  <<"]";
    graphics_.Terminate();
}

template<graphics::HRI Gfx>
void Renderer<Gfx>::Resize(const window::Resolution &res) {
    camera_.Resize(res);
    graphics_.Resize(res);
}
}