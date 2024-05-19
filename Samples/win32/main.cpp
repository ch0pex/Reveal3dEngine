

#include "core/scene.hpp"
#include "render/viewport.hpp"
#include "math/math.hpp"

#include "Samples/common/scripts.hpp"

using namespace reveal3d;
using namespace reveal3d::graphics;


LogLevel loglevel = logDEBUG;

_Use_decl_annotations_
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    {
        Timer timer;
        f32 time = timer.TotalTime();
        window::InitInfo windowInitInfo(L"Reveal3d", 1920, 1080);
        render::Viewport<dx::Graphics, window::Win32> viewport(windowInitInfo);
//        core::scene.AddEntityFromObj(relative(L"Assets/habitacion/muros.obj").c_str());
//        core::scene.AddEntityFromObj(relative(L"Assets/habitacion/aireacondicionado.obj").c_str());
//        core::scene.AddEntityFromObj(relative(L"Assets/habitacion/alfombrilla.obj").c_str());
//        core::scene.AddEntityFromObj(relative(L"Assets/habitacion/estanteria.obj").c_str());
//        core::scene.AddEntityFromObj(relative(L"Assets/habitacion/estanteria2.obj").c_str());
//        core::scene.AddEntityFromObj(relative(L"Assets/habitacion/monitor.obj").c_str());
//        core::scene.AddEntityFromObj(relative(L"Assets/habitacion/monitor_grande.obj").c_str());
//        core::scene.AddEntityFromObj(relative(L"Assets/habitacion/monitor_izquierdo.obj").c_str());
//        core::scene.AddEntityFromObj(relative(L"Assets/habitacion/mesa.obj").c_str());
//        core::scene.AddEntityFromObj(relative(L"Assets/habitacion/pata.obj").c_str());
//        core::scene.AddEntityFromObj(relative(L"Assets/habitacion/pc.obj").c_str());
//        core::scene.AddEntityFromObj(relative(L"Assets/habitacion/ps5.obj").c_str());
//        core::scene.AddEntityFromObj(relative(L"Assets/habitacion/switch.obj").c_str());
//        core::scene.AddEntityFromObj(relative(L"Assets/habitacion/teclado.obj").c_str());

        core::Entity human = core::scene.AddEntityFromObj(relative(L"Assets/habitacion/plane6.obj").c_str());
//        human.SetScale(10);
//        core::scene.addprimitive(reveal3d::core::geometry::cube);
//
//        for (u32 i = 0; i < 50; ++i) {
//            for (u32 j = 0; j < 10; ++j) {
//                for (u32 k = 0; k < 10; ++k) {
//                    core::EntityInfo info = {
//                            core::Transform({i * 1.5f, j * 1.5f, 1.5f * k}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}),
//                            human.GetGeometry()};
//                    core::Entity entity = core::scene.AddEntity(info);
//                    entity.AddScript<HumanScript>();
//
//                }
//            }
//        }

        core::scene.Init();
        viewport.Init();
        log(logDEBUG) << "Total Init time: " << timer.Diff(time);
        viewport.Run();
        log(logDEBUG) << "Media de fps: " << viewport.Time().AverageFps();

    }
#ifdef _DEBUG
//    graphics::dx::utl::ReportLiveDeviceObjs();
#endif
    return 0;
}