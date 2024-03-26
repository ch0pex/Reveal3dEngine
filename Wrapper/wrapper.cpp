#include "Engine/render/viewport.hpp"

#include <optional>

using namespace reveal3d;
using namespace reveal3d::graphics;
using namespace reveal3d::render;
using namespace reveal3d::window;
using namespace reveal3d::math;
using namespace reveal3d::core;
using Dx12 = graphics::dx::Graphics;

LogLevel loglevel = logDEBUG;


std::optional<Scene> scene;
std::optional<Viewport<Dx12, Win32>> viewport;

extern "C" __declspec(dllexport)
int CreateViewport(HWND host, i32 width, i32 heigth) {
    InitInfo initInfo = {
        .res = Resolution(width, heigth)
    };

    scene.emplace();
    viewport.emplace(initInfo, scene.value());
    return 0;
}

extern "C" __declspec(dllexport)
HWND GetWindowHandle() {
    return viewport->window.GetHwnd();
}
