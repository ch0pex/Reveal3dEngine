
if (WIN32)
    add_executable(Sample WIN32 win32/main.cpp)
else()
    add_executable(Sample unix/main.cpp)
endif()

target_link_libraries(Sample Engine)
target_include_directories(Sample PUBLIC ../Engine)
