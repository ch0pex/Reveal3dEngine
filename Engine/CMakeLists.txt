set(SOURCES
        core/scene.cpp
        core/geometry.cpp
        core/transform.cpp
        core/script.cpp
        core/entity.cpp
        render/renderer.cpp
        render/camera.cpp
        render/light.cpp
        common/timer.cpp
        config/config.cpp
        input/input.cpp
        content/primitives.cpp
        content/obj_parser.cpp
)

message("Renderer: ${GFX}")
message("Window: ${PLAT}")
aux_source_directory(graphics/${GFX} GRAPHICS)
aux_source_directory(graphics/${GFX}/resources GRAPHICS)
aux_source_directory(window/${PLAT} PLATFORM)
#message(Graphics Sources: ${GRAPHICS})
#message(Platform Sources: ${PLATFORM})


add_library(Engine STATIC ${SOURCES} ${GRAPHICS} ${PLATFORM})
target_include_directories(Engine PUBLIC .)
target_include_directories(Engine PUBLIC math/${PLAT})
target_include_directories(Engine PUBLIC graphics/${GFX})


if(D3D12)
    target_compile_definitions(Engine PRIVATE "UNICODE" "_UNICODE")
    target_link_libraries(Engine PRIVATE d3d12 dxgi d3dcompiler)
else()

endif()