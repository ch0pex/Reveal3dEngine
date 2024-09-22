/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_render_info.cpp
 * @version 1.0
 * @date 27/03/2024
 * @brief Short description
 *
 * Longer description
 */

#include "dx_render_info.hpp"

namespace reveal3d::graphics {

RenderElement::RenderElement(u32 id, dx12::BufferInitInfo &vertexInfo, dx12::BufferInitInfo &indexInfo)
        : meshId(id),
          vertexBuffer(vertexInfo),
          indexBuffer(indexInfo)
{
}

}