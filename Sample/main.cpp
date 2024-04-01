

#include "core/scene.hpp"
#include "render/viewport.hpp"
#include "window/window.hpp"
#include "math/math.hpp"
#include "input/input.hpp"

using namespace reveal3d;
using namespace reveal3d::core;

#if defined(D3D12)
#include "core/entity.hpp"
#include "graphics/directX12/dx_utils.hpp"
using Gfx = graphics::dx::Graphics;
#elif defined(VULKAN)
using Gfx = Vk::Graphics;
#endif

LogLevel loglevel = logDEBUG;

#if defined(_WIN32)

LRESULT WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

// Sample scripts for Movement and rotation
class RotationScript : public Script {
public:
    void Update(f32 dt) override {
        math::xvec3 rot = {0.0f, 180.0f, 0.0f};
        entity_->SetRotation(entity_->Rotation() + rot * dt);
    }
};

class MovementScript : public Script {
public:
    void Update(f32 dt) override {
        if (entity_->Position().GetZ() > 5.0f || entity_->Position().GetZ() < -5.0f) {
           pos_ = -pos_;
        }
        entity_->SetPosition(entity_->Position() + pos_ * dt);
    }
private:
    math::xvec3 pos_ = {0.0f, 0.0f, 1.0f};
};

_Use_decl_annotations_
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    {
        Scene scene;
        window::InitInfo windowInitInfo(L"Reveal3d", 1920, 1080, &WindowProc);
        Viewport<Gfx, window::Win32> viewport(windowInitInfo);

        Entity human = scene.AddEntityFromObj(L"D:\\Universidad\\tfg\\engine\\Reveal3d\\Assets\\human.obj");
        Entity room = scene.AddEntityFromObj(L"D:\\Universidad\\tfg\\engine\\Reveal3d\\Assets\\habitacion.obj");
        Entity cube = scene.AddPrimitive(Geometry::primitive::cube);

        RotationScript rotationScript;
        MovementScript movementScript;

        human.AddScript(rotationScript);
        human.AddScript(movementScript);
        room.SetScale(3);

        try {
            viewport.window.Create(viewport.renderer);
            viewport.renderer.Init(viewport.window.GetHwnd(), scene);
            viewport.window.Show();

            viewport.timer_.Reset();
            while(!viewport.window.ShouldClose()) {
                viewport.timer_.Tick();
                viewport.window.ClipMouse(viewport.renderer);
                viewport.renderer.Update(scene);
                scene.Update(viewport.timer_.DeltaTime());
                viewport.window.Update();
            }
            viewport.renderer.Destroy();
        } catch(std::exception &e) {
            viewport.renderer.Destroy();
            log(logERROR) << e.what();
            MessageBoxA(viewport.window.GetHwnd(), e.what(), NULL, MB_ICONERROR | MB_SETFOREGROUND);
            return EXIT_FAILURE;
        }
    }
#ifdef _DEBUG
    graphics::dx::utl::ReportLiveDeviceObjs();
#endif
    return 0;
}


LRESULT WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

    auto* renderer = reinterpret_cast<Renderer<Gfx>*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (message) {
        case WM_CREATE:
        {
            LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
            return 0;
        }
        case WM_ENTERSIZEMOVE:
        case WM_EXITSIZEMOVE:
        case WM_SIZE:
        {
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            const window::Resolution res(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
            renderer->Resize(res);
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT:
            try {
                renderer->Render();
            } catch (std::exception &e) {
                log(logERROR) << e.what();
                renderer->Destroy();
                PostQuitMessage(0);
            }
            return 0;
        case WM_KEYDOWN:
            input::KeyDown(wParam);
            return 0;
        case WM_KEYUP:
            input::KeyUp(wParam);
            return 0;
        case WM_MBUTTONDOWN:
        {
            input::cursor::shouldClip = true;
            SetCapture(hwnd);
            input::KeyDown(input::code::mouse_middle);
            return 0;
        }
        case WM_MBUTTONUP:
        {
            input::cursor::shouldClip = false;
            SetCapture(hwnd);
            input::KeyUp(input::code::mouse_middle);
            ReleaseCapture();
            return 0;
        }
        case WM_MOUSEMOVE:
        {
            input::cursor::pos = {(f32)GET_X_LPARAM(lParam), (f32)GET_Y_LPARAM(lParam)};
            input::MouseMove(wParam, input::cursor::pos);
            return 0;
        }
        case WM_RBUTTONDOWN:
            input::KeyDown(input::code::mouse_right, {(f32)GET_X_LPARAM(lParam), (f32)GET_Y_LPARAM(lParam)});
            return 0;
        case WM_RBUTTONUP:
            input::KeyUp(input::code::mouse_right, {(f32)GET_X_LPARAM(lParam), (f32)GET_Y_LPARAM(lParam)});
            return 0;
        case WM_LBUTTONDOWN:
            input::KeyDown(input::code::mouse_left, {(f32)GET_X_LPARAM(lParam), (f32)GET_Y_LPARAM(lParam)});
            return 0;
        case WM_LBUTTONUP:
            input::KeyUp(input::code::mouse_left, {(f32)GET_X_LPARAM(lParam), (f32)GET_Y_LPARAM(lParam)});
            return 0;

    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

#else

int main() {
    Viewport<VkGraphics, Glfw> viewport;
}

#endif