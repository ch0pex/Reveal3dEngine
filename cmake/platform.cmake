
if (WIN32)
    message("-- WINDOWS")
    message("-- Source dir: ${PROJECT_SOURCE_DIR}")
    include(cmake/platforms/windows.cmake)
elseif (UNIX)
    message("-- UNIX")
    message("-- Source dir: ${PROJECT_SOURCE_DIR}")
    include(cmake/platforms/windows.cmake)
endif ()

# Cross platform backends
include(cmake/backends/graphics/opengl.cmake)
include(cmake/backends/graphics/vulkan.cmake)
include(cmake/backends/window/glfw.cmake)
