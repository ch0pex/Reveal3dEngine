/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file scene.hpp
 * @version 1.0
 * @date 11/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "common/id.hpp"
#include "common/timer.hpp"
#include "content/primitives.hpp"
#include "geometry.hpp"
#include "script.hpp"
#include "transform.hpp"

#include <vector>

#include <deque>
namespace reveal3d::core {

struct EntityInfo {
    Transform transform;
    Geometry geometry;
};

//TODO: ids, generation and custom std::vector
class Scene {
public:
    Scene() = default;
    ~Scene();

    Entity AddEntity(math::vec3 pos);
    Entity AddEntity(EntityInfo &entity);
    Entity AddPrimitive(Geometry::primitive type);
    Entity AddEntityFromObj(const wchar_t *path);
    bool RemoveEntity(id_t id);

    INLINE std::string &GetName(u32 id) { return names_.at(id); }
    INLINE Transform &GetTransform(u32 id) { return transforms_.at(id); }
    INLINE Geometry &GetGeometry(u32 id) { return geometries_.at(id); }
    INLINE Script *GetScript(u32 id) { return scripts_.at(id); }

    [[nodiscard]] INLINE u32 NumEntities() const { return generations_.size(); }
    INLINE std::vector<Transform>& Transforms() { return transforms_; }
    INLINE std::vector<Geometry>& Geometries() { return geometries_; }
    Entity GetEntity(u32 id);

    void Init();
    void Update(f32 dt);
    void AddScript(Script *script, id_t id);

private:
    // Entities Ids
    std::vector<id_t> generations_;
    std::deque<id_t> freeIndices_;

    //Entities components
    std::vector<std::string> names_;
    std::vector<Transform> transforms_;
    std::vector<Geometry> geometries_;
    std::vector<Script *> scripts_;
};

extern Scene scene;

}
