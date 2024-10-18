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

#include <render/mesh.hpp>
#include <vector>
#include <string>


namespace reveal3d::content {

u32 GetDataFromObj(const std::string_view path, std::vector<render::Vertex> &vertices, std::vector<u32> &indices);
render::Mesh ImportObj(const std::string_view path);


}