set(SOURCES
        core/scene.cpp
        core/geometry.cpp
        core/transform.cpp
        core/script.cpp
        core/entity.cpp
        render/camera.cpp
        render/light.cpp
        common/timer.cpp
        config/config.cpp
        input/input.cpp
        content/primitives.cpp
        content/obj_parser.cpp
        window/glfw/glfw.cpp
)

set(HEADERS
        core/scene.hpp
        core/geometry.hpp
        core/transform.hpp
        core/script.hpp
        core/entity.hpp
        render/renderer.hpp
        render/camera.hpp
        render/light.hpp
        common/timer.hpp
        config/config.hpp
        input/input.hpp
        content/primitives.hpp
        content/obj_parser.hpp
        window/glfw/glfw.hpp
)
set(PROJECT_ROOT_DIR ${CMAKE_SOURCE_DIR})

add_library(Engine STATIC ${SOURCES} ${HEADERS})

find_package(OpenGL REQUIRED)
target_include_directories(Engine PRIVATE extern/include)

if (WIN32)
    #add_compile_options(/Wall /external:anglebrakets /external:W0)
    message("-- WINDOWS")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2")
    set(PLAT win32)
    set(EXTERN ${PROJECT_SOURCE_DIR}/Engine/extern/lib/${PLAT})
    target_compile_definitions(Engine PRIVATE "UNICODE" "_UNICODE")
    target_link_libraries(Engine PRIVATE d3d12 dxgi d3dcompiler ${OPENGL_LIBRARY} ${EXTERN}/glew.lib ${EXTERN}/glfw3.lib)

    aux_source_directory(graphics/directX12 GRAPHICS)
    aux_source_directory(graphics/directX12/resources GRAPHICS)
    aux_source_directory(window/win32 WINDOW)
    #add glew and glfw for windows
elseif (UNIX)
    message("-- UNIX" )
    set(PLAT unix)
#    find_package(GLM REQUIRED)
    find_package(GLEW REQUIRED)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native -O3 -ftree-vectorize -fprefetch-loop-arrays")
    set(EXTERN ${PROJECT_SOURCE_DIR}/Engine/extern/lib/${PLAT})
    target_link_libraries(Engine PRIVATE glfw ${GLEW_LIBRARIES} ${OPENGL_LIBRARY})

#    add_compile_options(-Werror -Wall -Wextra  -pedantic -pedantic-errors)
    #add glew, glfw and glm for linux
#    target_link_libraries(Engine PRIVATE ${EXTERN}/glew.t a ${EXTERN}/glfw3.lib)
endif()

aux_source_directory(graphics/opengl GRAPHICS)
aux_source_directory(graphics/vulkan GRAPHICS)
aux_source_directory(window/glfw WINDOW)
if (IMGUI)
    message("-- IMGUI support activated")
    target_compile_definitions(Engine PRIVATE IMGUI=1)
    aux_source_directory(extern/include/IMGUI IMGUI_SOURCE)
    aux_source_directory(extern/include/IMGUI/backends IMGUI_SOURCE)
    target_include_directories(Engine PUBLIC extern/include/IMGUI)
endif()
target_sources(Engine PUBLIC ${GRAPHICS} ${WINDOW} ${IMGUI_SOURCE})
#target_link_libraries(Engine PRIVATE )

target_include_directories(Engine PUBLIC .)
target_include_directories(Engine PUBLIC math/${PLAT})
target_include_directories(Engine PUBLIC extern/include)
target_compile_definitions(Engine PUBLIC PROJECT_ROOT_DIR="${PROJECT_ROOT_DIR}")

