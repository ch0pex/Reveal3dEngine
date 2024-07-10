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

#include <deque>
#include <set>
#include <vector>


namespace reveal3d::core {

class Entity {
public:
    Entity() : id_(id::invalid) {}
    explicit Entity(std::string& name);
    explicit Entity(const wchar_t *path);
    explicit Entity(id_t id);

    std::string& Name() const;
    Transform& Transform();
    Geometry& Geometry();
    Script Script();

    void SetName(std::string_view name);
    void SetTransform();
    void SetGeometry();
    void SetScript();

    INLINE u32 Id() const { return id_; }
    bool IsAlive();

private:
    void GenerateId();
    id_t id_;
};


class Scene {
public:
    struct Node {
        Entity entity;
        Entity parent;
        Entity firstChild;
        Entity next;
        Entity prev;
    };
    Scene() = default;
    ~Scene();

    Entity CreateEntity();
    void AddEntity(Entity entity);
    void AddChild(Entity child, Entity parent);

    Entity AddEntityFromObj(const wchar_t *path);
    bool RemoveEntity(id_t id);

    INLINE Entity GetEntity(id_t id) { return sceneGraph_.at(id::index(id)).entity; }
    INLINE u32 NumEntities() const { return sceneGraph_.size(); }
    INLINE Node& GetNode(id_t id) { return sceneGraph_.at(id::index(id)); }
    INLINE Node& Root() { return sceneGraph_.at(0); }
    INLINE const std::vector<Scene::Node>& Graph() const { return sceneGraph_; }

    std::vector<Transform>& Transforms();
    std::set<id_t>& DirtyTransforms();
    std::vector<Geometry>& Geometries();

    void Init();
    void Update(f32 dt);
    void AddScript(Script *script, id_t id);

private:
    void UpdateTransforms();
    void UpdateGeometries();
    // Entity graph
    Node* lastNode;
    std::vector<Scene::Node> sceneGraph_;
};

extern Scene scene;

}
