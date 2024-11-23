set(GLFW_SOURCES
        window/glfw/glfw.cpp
)

set(GLFW_HEADERS
        window/glfw/glfw.hpp
)

target_sources(Reveal3d PUBLIC ${GLFW_SOURCES} ${GLFW_HEADERS})
