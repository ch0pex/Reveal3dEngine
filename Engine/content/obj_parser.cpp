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

#include <unordered_map>
#include <fstream>


namespace reveal3d::content {

struct FaceElem {
    struct Hash {
        size_t operator()(const FaceElem& p) const
        {
            size_t val;
            std::stringstream string;
            string << p.posIndex << p.uvIndex << p.normalIndex;
            string >> val;
            return val;
        }
    };
    bool operator==(const FaceElem& p) const
    {
        return posIndex == p.posIndex && uvIndex == p.uvIndex && normalIndex == p.normalIndex;
    }

    u16 posIndex;
    u16 uvIndex;
    u16 normalIndex;
};

static void GetPoly(std::string &line, std::vector<FaceElem> &primitives) {
    u16 elem[4][3];
    sscanf(line.c_str(), "f %hu/%hu/%hu %hu/%hu/%hu %hu/%hu/%hu %hu/%hu/%hu",
           &elem[0][0], &elem[0][1], &elem[0][2],
           &elem[1][0], &elem[1][1], &elem[1][2],
           &elem[2][0], &elem[2][1], &elem[2][2],
           &elem[3][0], &elem[3][1], &elem[3][2]);
    for(u32 i = 0; i < 4; ++i) {
        primitives.push_back({elem[i][0], elem[i][1], elem[i][2]});
    }
}

static void GetTriangle(std::string &line, std::vector<FaceElem> &primitives) {
    u16 elem[3][3];
    sscanf(line.c_str(), "f %hu/%hu/%hu %hu/%hu/%hu %hu/%hu/%hu",
           &elem[0][0], &elem[0][1], &elem[0][2],
           &elem[1][0], &elem[1][1], &elem[1][2],
           &elem[2][0], &elem[2][1], &elem[2][2]);
    for(u32 i = 0; i < 3; ++i) {
        primitives.push_back({elem[i][0], elem[i][1], elem[i][2]});
    }
}


u16 GetDataFromObj(const wchar_t *path, std::vector<render::Vertex> &vertices, std::vector<u16> &indices, u16 index) {
    std::ifstream file(path);
    std::vector<math::vec3> positions;
    std::vector<math::vec3> normals;
    std::vector<math::vec2> uvs;
    std::vector<FaceElem> primitives;
    char c[2];
    std::string line;

    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo" << std::endl;
        return 0;
    }

    while (std::getline(file, line)) {
        if (line[0] == 'v') {
            if (line[1] == 'n') {
                math::vec3 normal;
                std::stringstream(line) >> c[0] >> c[1] >> normal.x >> normal.y >> normal.z;
                normal.z = -normal.z;
                normal.y = -normal.y;
                normals.push_back(normal);
            } else if (line[1] == 't') {
                math::vec2 uv;
                std::stringstream(line) >> c[0] >> c[1] >> uv.x >> uv.y;
                uvs.push_back(uv);
            } else {
                math::vec3 pos;
                std::stringstream(line) >> c[0] >> pos.x >> pos.y >> pos.z;
                pos.y = -pos.y;
                positions.push_back(pos);
            }
        } else if (line[0] == 'f'){
            GetTriangle(line, primitives);
        }
    }

    render::Vertex vert;
    std::unordered_map<FaceElem, u16, FaceElem::Hash> cache;
    for(u32 i = 0; i < primitives.size(); ++i) {
        if (cache.find(primitives[i]) == cache.end()) {
            vert.pos = positions[primitives[i].posIndex - 1U];
            vert.normal = normals[primitives[i].normalIndex - 1U];
//            vert.uv = uvs[primitives[i].uvIndex - 1U];
//            vert.color = { 0.8f, 0.0f, 0.0f, 0.0f };
            vertices.push_back(vert);
            indices.push_back(index);
            cache[primitives[i]] = index++;
        } else {
            indices.push_back(cache[primitives[i]]);
        }
    }

    file.close();
    return index;
}


} // namespace reveal3d::content
