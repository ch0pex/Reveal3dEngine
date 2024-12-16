
set(PLAT win32)
set(EXTERN ${PROJECT_SOURCE_DIR}/extern/lib/${PLAT})
add_compile_options(/Wall /Wextra /external:anglebrakets /external:W0 /experimental:module /concepts /diagnostics:caret)

include(cmake/backends/graphics/directx12.cmake)
include(cmake/backends/window/win32.cmake)

target_link_libraries(Reveal3d PRIVATE d3d12 dxgi d3dcompiler ${OPENGL_LIBRARY} ${EXTERN}/glew.lib ${EXTERN}/glfw3.lib)
