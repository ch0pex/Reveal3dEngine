set(CORE
        core/scene.hpp
        core/components/geometry.hpp
        core/components/transform.hpp
        core/components/script.hpp
        core/components/metadata.hpp
        core/components/light.hpp
        core/pooling/pool.hpp
        core/pooling/transform_pool.hpp
        core/pooling/script_pool.hpp
        core/pooling/metadata_pool.hpp
        core/pooling/geometry_pool.hpp
        core/pooling/rigidbody_pool.hpp
        core/pooling/light_pool.hpp
        core/components/rigidbody.hpp
        core/components/component.hpp
        core/components/concepts.hpp
        core/pooling/concepts.hpp
        core/pooling/pools_map.hpp
)

set(CONTENT
        content/formats/obj/obj_parser.cpp
        content/formats/obj/obj_parser.hpp
        content/content.hpp
        content/primitives.hpp
)

set(COMMON
        common/timer.hpp
        common/id.hpp
        common/vector.hpp
        common/string_literals.hpp
        common/primitive_types.hpp
        common/tuple.hpp
        common/logger.hpp
        common/platform.hpp
)

set(RENDER
        render/renderer.hpp
        render/camera.hpp
        render/light.hpp
        render/material.hpp
)

set(INPUT
        input/input.hpp
        input/input_types.hpp
)

set(CONFIG
        config/config.hpp
)

set(MATH
        math/math.hpp
)

set(WINDOW
        window/window.hpp
        window/window_info.hpp
)

target_sources(Reveal3d PUBLIC ${CORE} ${CONTENT} ${COMMON} ${RENDER} ${INPUT} ${CONFIG} ${MATH} ${WINDOW})