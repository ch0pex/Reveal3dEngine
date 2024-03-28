/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_render_info.hpp
 * @version 1.0
 * @date 27/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "resources/dx_resources.hpp"

namespace reveal3d::graphics::dx {

struct RenderInfo {
    RenderInfo(BufferInitInfo &vertexInfo, BufferInitInfo &indexInfo);
    VertexBuffer vertexBuffer;
    IndexBuffer indexBuffer;
    DescriptorHandle handles[frameBufferCount];
    u8 frameDirty { 0 };
};

}
