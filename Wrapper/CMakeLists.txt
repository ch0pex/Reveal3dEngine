
add_library(Wrapper SHARED
        dllmain.cpp
        wrapper.cpp
)
target_link_libraries(Wrapper Engine)
target_include_directories(Wrapper PUBLIC ../Engine)
