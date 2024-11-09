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
#include "common/timer.hpp"

#include <fstream>
#include <string>
#include <unordered_map>


namespace reveal3d::content {

struct FaceElem {
  struct Hash {
    size_t operator()(FaceElem const& p) const { return ((p.pos_index << 16U) | (p.uv_index)); }
  };
  bool operator==(FaceElem const& p) const {
    return pos_index == p.pos_index && uv_index == p.uv_index && normal_index == p.normal_index;
  }

  u32 pos_index;
  u32 uv_index;
  u32 normal_index;
};

static void getPoly(std::string& line, std::vector<FaceElem>& primitives) {
  u32 elem[4][3];

  std::sscanf(
      line.c_str(), "f %u/%u/%u %u/%u/%u %u/%u/%u %u/%u/%u", &elem[0][0], &elem[0][1], &elem[0][2], &elem[1][0],
      &elem[1][1], &elem[1][2], &elem[2][0], &elem[2][1], &elem[2][2], &elem[3][0], &elem[3][1], &elem[3][2]
  );
  for (u32 i = 0; i < 4; ++i) {
    primitives.push_back({elem[i][0], elem[i][1], elem[i][2]});
  }
}

static void getTriangle(std::string& line, std::vector<FaceElem>& primitives) {
  u32 elem[3][3];
  std::sscanf(
      line.c_str(), "f %u/%u/%u %u/%u/%u %u/%u/%u", &elem[0][0], &elem[0][1], &elem[0][2], &elem[1][0], &elem[1][1],
      &elem[1][2], &elem[2][0], &elem[2][1], &elem[2][2]
  );
  for (u32 i = 0; i < 3; ++i) {
    primitives.push_back({elem[i][0], elem[i][1], elem[i][2]});
  }
}


std::optional<render::Mesh> import_obj(std::string_view const path) {
  render::Mesh mesh;
  auto const start = Timer::now();
  std::ifstream file(path.data());
  std::vector<math::vec3> positions;
  std::vector<math::vec3> normals;
  std::vector<math::vec2> uvs;
  std::vector<FaceElem> primitives;
  std::string line;

  if (!file.is_open()) {
    logger(LogError) << "Error opening obj file";
    return std::nullopt;
  }

  while (std::getline(file, line)) {
    if (line.empty())
      continue;

    char const prefix = line[0];
    if (prefix == 'v') {
      if (line[1] == 'n') {
        math::vec3 normal;
        if (std::sscanf(line.c_str(), "vn %f %f %f", &normal.x, &normal.y, &normal.z) == 3) {
          normal.y = -normal.y;
          normals.push_back(normal);
        }
      }
      else if (line[1] == 't') {
        math::vec2 uv;
        if (std::sscanf(line.c_str(), "vt %f %f", &uv.x, &uv.y) == 2) {
          uvs.push_back(uv);
        }
      }
      else {
        math::vec3 pos;
        if (std::sscanf(line.c_str(), "v %f %f %f", &pos.x, &pos.y, &pos.z) == 3) {
          pos.y = -pos.y;
          positions.push_back(pos);
        }
      }
    }
    else if (prefix == 'f') {
      getTriangle(line, primitives);
    }
  }

  u32 index = 0;
  render::Vertex vert;
  std::unordered_map<FaceElem, u32, FaceElem::Hash> cache;
  for (auto& primitive: primitives) {
    if (auto [it, inserted] = cache.emplace(primitive, index); inserted) {
      vert.pos    = positions[primitive.pos_index - 1U];
      vert.normal = normals[primitive.normal_index - 1U];
      mesh.vertices.push_back(vert);
      mesh.indices.push_back(index++);
    }
    else {
      mesh.indices.push_back(it->second);
    }
  }

  file.close();
  logger(LogInfo) << "Time to import: " << Timer::diff(start) << "s";
  return std::move(mesh);
}

} // namespace reveal3d::content
