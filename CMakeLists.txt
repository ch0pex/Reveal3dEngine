cmake_minimum_required(VERSION 3.28)
project(Reveal3d LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(PROJECT_ROOT_DIR ${CMAKE_SOURCE_DIR})

include_directories(PUBLIC .)

add_library(Reveal3d STATIC)

include(cmake/source.cmake)
include(cmake/extern.cmake)
include(cmake/platform.cmake)
include(cmake/imgui.cmake)

target_include_directories(Reveal3d PUBLIC .)
target_include_directories(Reveal3d PUBLIC math/${PLAT})
target_include_directories(Reveal3d PUBLIC extern/include)
target_compile_definitions(Reveal3d PUBLIC PROJECT_ROOT_DIR="${PROJECT_ROOT_DIR}")
