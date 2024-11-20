cmake_minimum_required(VERSION 3.28)
project(Reveal3d LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(PUBLIC .)

add_library(Reveal3d STATIC)


set(SOURCES
        render/light.cpp
        content/formats/obj/obj_parser.cpp
)

set(HEADERS
        config/config.hpp
        core/scene.hpp
        core/components/geometry.hpp
        core/components/transform.hpp
        core/components/script.hpp
        core/components/metadata.hpp
        render/renderer.hpp
        render/camera.hpp
        render/light.hpp
        common/timer.hpp
        input/input.hpp
        content/primitives.hpp
        content/formats/obj/obj_parser.hpp
        window/glfw/glfw.hpp
        common/id.hpp
        render/material.hpp
        content/content.hpp
        core/pooling/pool.hpp
        core/pooling/transform_pool.hpp
        core/pooling/script_pool.hpp
        core/pooling/metadata_pool.hpp
        core/pooling/geometry_pool.hpp
        core/concepts.hpp
        common/vector.hpp
        common/string_literals.hpp
        common/string_literals.hpp
        core/pooling/rigidbody_pool.hpp
        core/components/rigidbody.hpp
        math/utils.hpp
        input/input_types.hpp
)

set(PROJECT_ROOT_DIR ${CMAKE_SOURCE_DIR})

target_sources(Reveal3d
        PUBLIC
        ${SOURCES} ${HEADERS}
)


include(FetchContent)
FetchContent_Declare(
        tomlplusplus
        GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
        GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(tomlplusplus)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 10.1.1  # Cambia a la versión de fmt que prefieras
)

FetchContent_MakeAvailable(fmt)

target_link_libraries(Reveal3d PUBLIC tomlplusplus::tomlplusplus)
target_link_libraries(Reveal3d PUBLIC fmt::fmt)
find_package(OpenGL REQUIRED)
target_include_directories(Reveal3d PRIVATE extern/include)


if (WIN32)
    add_compile_options(/Wall /external:anglebrakets /external:W0)
    message("-- WINDOWS")
    message("-- Source dir: ${PROJECT_SOURCE_DIR}")
    add_compile_options(/experimental:module)
    set(PLAT win32)
    set(EXTERN ${PROJECT_SOURCE_DIR}/extern/lib/${PLAT})
    #    target_compile_definitions(Reveal3d PRIVATE "UNICODE" "_UNICODE")
    target_link_libraries(Reveal3d PRIVATE d3d12 dxgi d3dcompiler ${OPENGL_LIBRARY} ${EXTERN}/glew.lib ${EXTERN}/glfw3.lib)

    aux_source_directory(graphics/directX12 GRAPHICS)
    aux_source_directory(graphics/directX12/resources GRAPHICS)
    aux_source_directory(graphics/directX12/utils GRAPHICS)
    aux_source_directory(window/win32 WINDOW)
    #addId glew and glfw for windows
elseif (UNIX)
    message("-- UNIX")
    set(PLAT unix)
    #    find_package(GLM REQUIRED)
    find_package(GLEW REQUIRED)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native -O3 -ftree-vectorize -fprefetch-loop-arrays")
    set(EXTERN ${PROJECT_SOURCE_DIR}/Engine/extern/lib/${PLAT})
    target_link_libraries(Reveal3d PRIVATE glfw ${GLEW_LIBRARIES} ${OPENGL_LIBRARY})

    #    add_compile_options(-Werror -Wall -Wextra  -pedantic -pedantic-errors)
    #addId glew, glfw and glm for linux
    #    target_link_libraries(Reveal3d PRIVATE ${EXTERN}/glew.t a ${EXTERN}/glfw3.lib)
endif ()

aux_source_directory(graphics/opengl GRAPHICS)
aux_source_directory(graphics/vulkan GRAPHICS)
aux_source_directory(window/glfw WINDOW)

if (IMGUI)
    message("-- IMGUI support activated")
    target_compile_definitions(Reveal3d PUBLIC IMGUI=1)
    aux_source_directory(extern/include/IMGUI IMGUI_SOURCE)
    aux_source_directory(extern/include/IMGUI/backends IMGUI_SOURCE)
    target_include_directories(Reveal3d PUBLIC extern/include/IMGUI)
endif ()
target_sources(Reveal3d PUBLIC ${GRAPHICS} ${WINDOW} ${IMGUI_SOURCE})

target_include_directories(Reveal3d PUBLIC .)
target_include_directories(Reveal3d PUBLIC math/${PLAT})
target_include_directories(Reveal3d PUBLIC extern/include)
target_compile_definitions(Reveal3d PUBLIC PROJECT_ROOT_DIR="${PROJECT_ROOT_DIR}")
