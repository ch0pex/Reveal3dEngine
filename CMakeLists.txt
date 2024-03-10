cmake_minimum_required(VERSION 3.22)
project(Reveal3dEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_VERBOSE_MAKEFILE ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


include_directories(PUBLIC .)

if (WIN32)
    message("-- WINDOWS")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /arch:AVX512")
    set(PLAT win32)
    #add_compile_options(/Wall /external:anglebrakets /external:W0)
elseif (UNIX)
    message("-- UNIX" )
    add_compile_options(-Werror -Wall -Wextra  -pedantic -pedantic-errors)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native -O3 -ftree-vectorize -fprefetch-loop-arrays")
    set(PLAT unix)
endif()

if (D3D12)
    add_definitions(-DD3D12=1)
    set(GFX directx12)
elseif(VULKAN)
    add_definitions(-DVULKAN=1)
    set(GFX vulkan)
endif()

#find_program(CLANG_TIDY_EXE NAMES clang-tidy)
#set(CMAKE_CXX_CLANG_TIDY "C:/Program Files/LLVM/bin/clang-tidy.exe" -header-filter=.*)
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(Sample)
add_subdirectory(Engine)
add_subdirectory(Wrapper)
add_subdirectory(Test)

