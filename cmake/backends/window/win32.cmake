
set(WIN32_SOURCES
        window/win32/win32.cpp
)

set(WIN32_HEADERS
        window/win32/win32.hpp
)

target_sources(Reveal3d PUBLIC ${WIN32_SOURCES} ${WIN32_HEADERS})
