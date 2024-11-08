/************************************************************************
 * copyright (c) 2024 alvaro cabrera barrio
 * this code is licensed under mit license (see license.txt for details)
 ************************************************************************/
/**
 * @file obj_parser.hpp
 * @version 1.0
 * @date 26/03/2024
 * @brief obj and fbx functions
 *
 * methods to parse obj and fbx functions
 *
 */

#pragma once

#include "render/mesh.hpp"

#include <string_view>


namespace reveal3d::content {

u32 get_data_from_obj(std::string_view path, std::vector<render::Vertex>& vertices, std::vector<u32>& indices);
render::Mesh import_obj(std::string_view path);


} // namespace reveal3d::content
