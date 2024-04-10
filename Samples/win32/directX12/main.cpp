

#include "core/scene.hpp"
#include "render/viewport.hpp"
#include "math/math.hpp"

#include "Samples/common/scripts.hpp"

using namespace reveal3d;
using namespace reveal3d::graphics;


LogLevel loglevel = logDEBUG;


LRESULT WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);



_Use_decl_annotations_
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    {

        Timer timer;
        f32 time = timer.TotalTime();
        window::InitInfo windowInitInfo(L"Reveal3d", 1920, 1080);
        Viewport<dx::Graphics, window::Win32> viewport(windowInitInfo);
        //        Viewport<opengl::Graphics, window::Win32> viewport(windowInitInfo);

        //        Viewport<opengl::Graphics, window::Glfw> viewport(windowInitInfo);
        //        Viewport<dx::Graphics, window::Glfw> viewport(windowInitInfo);


        core::Entity human = core::scene.AddEntityFromObj(relative(L"Assets/human.obj").c_str());
        core::Entity room = core::scene.AddEntityFromObj(relative(L"Assets/habitacion.obj").c_str());
        core::Scene *myScene = &core::scene;

        //        MovementScript movementScript;
        for (u32 i = 0; i < 10; ++i)
            for (u32 j = 0; j < 10; ++j) {
                for (u32 k = 0; k < 20; ++k) {
                    core::EntityInfo info = {
                            core::Transform({i * 1.5f, j * 1.5f, 1.5f * k}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}),
                            human.GetGeometry()};
                    core::Entity entity = core::scene.AddEntity(info);
                    entity.AddScript<HumanScript>();
                }
            }


//        human.AddScript(rotationScript);
//        human.AddScript(movementScript);
//        human.AddMesh(core::Geometry::cube);
//        cube.AddScript(rotationScript);
//        room.SetScale(3);
//        room.SetPosition(0.0f, 5.0f,0.0f);

        try {
            viewport.window.Create(viewport.renderer);
            viewport.renderer.Init(viewport.window.GetHwnd());
            viewport.window.Show();

            log(logDEBUG) << "Total Init time: " << timer.Diff(time);

            viewport.timer_.Reset();
            while(!viewport.window.ShouldClose()) {
                viewport.timer_.Tick();
                viewport.window.ClipMouse(viewport.renderer);
                viewport.renderer.Update();
                core::scene.Update(viewport.timer_.DeltaTime());
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