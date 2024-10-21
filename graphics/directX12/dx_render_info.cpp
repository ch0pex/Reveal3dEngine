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

RenderElement::RenderElement(core::Entity entity, dx12::BufferInitInfo &vertex_info, dx12::BufferInitInfo &index_info) :
    entity(entity), vertex_buffer(vertex_info), index_buffer(index_info) {}
}