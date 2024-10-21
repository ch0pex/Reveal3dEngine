/************************************************************************
* copyright (c) 2024 alvaro cabrera barrio
* this code is licensed under mit license (see license.txt for details)
************************************************************************/
/**
* @file obj_parser.hpp
* @version 1.0
* @date 26/03/2024
* @brief Obj parser 
*
* Functions to parse Wavefront (.obj) files
*
*/

#include "obj_parser.hpp"

#include <codecvt>
#include <fstream>
#include <locale>
#include <string>
#include <unordered_map>


namespace reveal3d::content {

struct FaceElem {
    struct Hash {
        size_t operator()(const FaceElem& p) const
        {
            return ((p.pos_index << 16U) | (p.uv_index));
        }
    };
    bool operator==(const FaceElem& p) const
    {
        return pos_index == p.pos_index && uv_index == p.uv_index && normal_index == p.normal_index;
    }

    u32 pos_index;
    u32 uv_index;
    u32 normal_index;
};

static void getPoly(std::string &line, std::vector<FaceElem> &primitives) {
    u32 elem[4][3];
    sscanf(line.c_str(), "f %u/%u/%u %u/%u/%u %u/%u/%u %u/%u/%u",
           &elem[0][0], &elem[0][1], &elem[0][2],
           &elem[1][0], &elem[1][1], &elem[1][2],
           &elem[2][0], &elem[2][1], &elem[2][2],
           &elem[3][0], &elem[3][1], &elem[3][2]);
    for(u32 i = 0; i < 4; ++i) {
        primitives.push_back({elem[i][0], elem[i][1], elem[i][2]});
    }
}

static void getTriangle(std::string &line, std::vector<FaceElem> &primitives) {
    u32 elem[3][3];
    sscanf(line.c_str(), "f %u/%u/%u %u/%u/%u %u/%u/%u",
           &elem[0][0], &elem[0][1], &elem[0][2],
           &elem[1][0], &elem[1][1], &elem[1][2],
           &elem[2][0], &elem[2][1], &elem[2][2]);
    for(u32 i = 0; i < 3; ++i) {
        primitives.push_back({elem[i][0], elem[i][1], elem[i][2]});
    }
}


u32 getDataFromObj(const std::string_view path, std::vector<render::Vertex> &vertices, std::vector<u32> &indices) {
#ifndef WIN32
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string path_str = converter.to_bytes(path);
    std::ifstream file(path_str);
#else
    std::ifstream file(path.data());
#endif
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
//                normal.z = -normal.z;
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
            getTriangle(line, primitives);
        }
    }

    u32 index = 0;
    render::Vertex vert;
    std::unordered_map<FaceElem, u32, FaceElem::Hash> cache;
    for(u64 i = 0; i < primitives.size(); ++i) {
        if (cache.find(primitives[i]) == cache.end()) {
            vert.pos = positions[primitives[i].pos_index - 1U];
            vert.normal = normals[primitives[i].normal_index - 1U];
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

render::Mesh importObj(const std::string_view path) {
   render::Mesh mesh;
   getDataFromObj(path, mesh.vertices, mesh.indices);
   return std::move(mesh);
}

} // namespace reveal3d::content
