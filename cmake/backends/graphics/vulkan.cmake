set(VULKAN_DIR graphics/vulkan)

set(VULKAN_SOURCES
        ${VULKAN_DIR}/vk_graphics_core.cpp
)

set(VULKAN_HEADERS
        ${VULKAN_DIR}/vk_graphics_core.hpp
)

target_sources(Reveal3d PUBLIC ${VULKAN_SOURCES} ${VULKAN_HEADERS})

