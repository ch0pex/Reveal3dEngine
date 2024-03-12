

#include "render/viewport.hpp"
#include "window/window.hpp"

#include "math/math.hpp"


using namespace reveal3d::graphics;
using namespace reveal3d::render;
using namespace reveal3d::window;
using namespace reveal3d::math;

#if defined(D3D12)
#include "graphics/directX12/dx_utils.hpp"
using Gfx = dx::Graphics;
#elif defined(VULKAN)
using Gfx = Vk::Graphics;
#endif

LogLevel loglevel = logDEBUG;

#if defined(_WIN32)
_Use_decl_annotations_ int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    {
        InitInfo windowInitInfo = { };
        //Scene myScene;
        //scene.addEntity(BOX)
        //scene.addEntity(OBJ, path)
        Viewport<Gfx, Win32> viewport(windowInitInfo);

        viewport.Run();
    }
#ifdef _DEBUG
    dx::utl::ReportLiveDeviceObjs();
#endif

}

#else

int main() {
    Viewport<VkGraphics, Glfw> viewport;
}

#endif
