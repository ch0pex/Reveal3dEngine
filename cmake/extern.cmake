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
