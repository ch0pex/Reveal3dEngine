

#include "core/scene.hpp"
#include "render/viewport.hpp"
#include "math/math.hpp"

#include "Samples/common/scripts.hpp"

//#include "IMGUI/imgui.h"
//#include "IMGUI/backends/imgui_impl_win32.h"
//#include "IMGUI/backends/imgui_impl_dx12.h"
//#include "IMGUI/imgui_internal.h"

using namespace reveal3d;
using namespace reveal3d::graphics;


LogLevel loglevel = logDEBUG;


i32 main() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //    io.ConfigViewportsNoAutoMerge = true;
    //    io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends

    window::InitInfo windowInitInfo(L"Reveal3d", 1920, 1080);
    render::Viewport<dx::Graphics, window::Win32> viewport(windowInitInfo);
    core::Entity human = core::scene.AddEntityFromObj(relative(L"Assets/human.obj").c_str());

    core::scene.Init();
    viewport.window.Create(viewport.renderer);
    viewport.renderer.Init(viewport.window.GetHandle());

    ImGui_ImplWin32_Init(viewport.window.GetHandle().hwnd);
    ImGui_ImplDX12_Init(viewport.renderer.Graphics().GetDevice(), 3,
                        DXGI_FORMAT_R8G8B8A8_UNORM, viewport.renderer.Graphics().GetHeaps().srv.Get(),
                        viewport.renderer.Graphics().GetHeaps().srv.CpuStart(),
                        viewport.renderer.Graphics().GetHeaps().srv.GpuStart() );

    viewport.window.Show();
    try {
        viewport.timer.Reset();
        while(!viewport.window.ShouldClose()) {
            viewport.timer.Tick();
            viewport.window.ClipMouse(viewport.renderer);
            viewport.renderer.Update();
            core::scene.Update(viewport.timer.DeltaTime());
            viewport.window.Update();
//            viewport.renderer.Render();
#ifdef IMGUI

#endif
        }
        viewport.renderer.Destroy();
        ImGui_ImplDX12_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    } catch(std::exception &e) {
        viewport.renderer.Destroy();
        log(logERROR) << e.what();
        //        MessageBoxA(window.GetHandle().hwnd, e.what(), NULL, MB_ICONERROR | MB_SETFOREGROUND);
    }
}


/*
_Use_decl_annotations_
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    {
        Timer timer;
        const f32 time = timer.TotalTime();
        window::InitInfo windowInitInfo(L"Reveal3d", 1920, 1080);
        render::Viewport<dx::Graphics, window::Win32> viewport(windowInitInfo);
        core::Entity human = core::scene.AddEntityFromObj(relative(L"Assets/human.obj").c_str());

        for (u32 i = 0; i < 10; ++i) {
            for (u32 j = 0; j < 10; ++j) {
                for (u32 k = 0; k < 10; ++k) {
                    core::EntityInfo info = {
                            core::Transform({i * 1.5f, j * 1.5f, 1.5f * k}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}),
                            human.GetGeometry()};
                    core::Entity entity = core::scene.AddEntity(info);
                    entity.AddScript<HumanScript>();

                }
            }
        }

        core::scene.Init();
        viewport.Init();
        log(logDEBUG) << "Total Init time: " << timer.Diff(time);
//        log(logDEBUG) << "Media de fps: " << viewport.BenchMark(30U);
        viewport.Run();

    }
#ifdef _DEBUG
//    graphics::dx::utl::ReportLiveDeviceObjs();
#endif
    return 0;
}
*/