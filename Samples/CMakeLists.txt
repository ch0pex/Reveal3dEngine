
if (WIN32)
    add_executable(Sample win32/main.cpp)
else()
    add_executable(Sample unix/main.cpp)
endif()

target_compile_definitions(Sample PRIVATE IMGUI=1)
target_link_libraries(Sample Engine)
target_include_directories(Sample PUBLIC ../Engine)

