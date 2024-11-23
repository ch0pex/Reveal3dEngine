set(OPENGL_DIR graphics/opengl)

set(OPENGL_SOURCES
        ${OPENGL_DIR}/gl_graphics_core.cpp
        ${OPENGL_DIR}/gl_render_info.cpp
        ${OPENGL_DIR}/gl_render_layers.cpp
)

set(OPENGL_HEADERS
        ${OPENGL_DIR}/gl_graphics_core.hpp
        ${OPENGL_DIR}/gl_render_info.hpp
        ${OPENGL_DIR}/gl_render_layers.hpp
)

target_sources(Reveal3d PUBLIC ${OPENGL_SOURCES} ${OPENGL_HEADERS})
