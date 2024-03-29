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

#include "obj_parser.hpp"

#include <fstream>


namespace reveal3d::content {

void GetDataFromObj(const wchar_t *path, std::vector<render::Vertex> &vertices, std::vector<u16> &indices) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo" << std::endl;
        return;
    }

    std::string line;

    u32 i = 0;
    //Header
    while (std::getline(file, line)) {
        if (line[0] == 'v') {
            render::Vertex vert;
            sscanf(line.c_str(), "v %f %f %f", &vert.pos.x, &vert.pos.y, &vert.pos.z);
            vert.color = {static_cast<float>(cos(++i)), static_cast<float>(sin(++i)), 0.3f, 0.0f};
            vertices.push_back(vert);

        } else if (line[0] == 'f'){
            u16 i,j,k = 0;
            sscanf(line.c_str(), "f %hu %hu %hu", &i, &j, &k);
            indices.emplace_back(i - 1U);
            indices.emplace_back(j - 1U);
            indices.emplace_back(k - 1U);
        }
    }

    file.close();

}


}