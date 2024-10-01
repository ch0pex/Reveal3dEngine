/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file scene.hpp
 * @version 1.0
 * @date 11/03/2024
 * @brief ECS  
 *
 * Entity component system main header file. 
 * In Reveal3D componentes just holds and ID that points to the real data.
 * Real data is compacted to avoid cache misses, this logic is handled by components pools
 *
 * *********************************************** Components pool *****************************************************
 * ************************** IDs **************************************************** Data ****************************
 *                                                          *                                                          *
 * Enity IDs        |  00   | 01    |  02   |  03  | ...    * Enity IDs        |  00   | 01    |  02   |  03  | ...    * 
 *                                                          *                                                          *
 * --------------------- Components IDs-------------------- * ------------------- Components Data -------------------- *
 *                  ------- ------- ------- ------          *                  ------- ------- -------                 *
 * Transform IDs   |   00  |  01   |  FF   |  02  |  ...    * Transforms      |       |       |       |  ...           *
 *                  ------- ------- ------- ------          *                  ------- ------- -------                 *
 *                  ------- ------- ------- ------          *                  ------- ------- -------                 *
 * Geometry IDs    |  00   |  FF   |  01   |  02  |  ...    * Geometries      |       |       |       |  ...           *
 *                  ------- ------- ------- ------          *                  ------- ------- -------                 *
 *                  ------- ------- ------- ------          *                  ------- -------                         *
 * Script IDs      |  00   |  FF   |  FF   |  01   |  ...   * Scripts         |       |       |  ...                   *
 *                  ------- ------- ------- ------          *                  ------- -------                         *
 * *********************************************************************************************************************
 */

#pragma once

#include "common/common.hpp"
#include "content/primitives.hpp"
#include "render/light.hpp"
#include "components.hpp"

#include <deque>
#include <set>
#include <vector>
#include <utility>

namespace reveal3d::core {

class Entity {
public:
    Entity() : id_(id::invalid) {}
    explicit Entity(id_t id);

    template<component T> T Component();
    template<component T> T AddComponent();
    template<component T> T AddComponent(T::InitInfo&& initInfo);
    template<component T> void RemoveComponent();

    [[nodiscard]] INLINE u32 Id() const { return id_; }
    bool IsAlive();

private:
    id_t id_;
};


class Scene {
public:
    struct Node {
        std::vector<id_t> GetChildren();
        Entity entity;
        Entity parent;
        Entity firstChild;
        Entity next;
        Entity prev;
    };
    Scene();
    ~Scene();

    Entity NewEntity();
    Entity NewChildEntity(Entity parent);
    Entity NewChildEntity(id_t parent);
    Entity RemoveEntity(id_t id);
    bool   IsEntityAlive(id_t id);

    /******************************* Scene Graph methods ******************************/
    [[nodiscard]] INLINE const std::vector<Scene::Node>& Graph() const { return sceneGraph_; }
    [[nodiscard]] INLINE u32 Count() const { return aliveCount_; }
    INLINE Entity GetEntity(id_t id) { return sceneGraph_.at(id::index(id)).entity; }
    INLINE Node& GetNode(id_t id) { return sceneGraph_.at(id::index(id)); }
    INLINE Node& Root() { return sceneGraph_.at(rootNode_); }

    template<component T>
    decltype(auto) ComponentPool() noexcept;

    void Init();
    void Update(f32 dt);

private:
    void RemoveNode(id_t id);

    /************ Entities scene ***********/

    id::Factory                 idFactory_;
    std::vector<Scene::Node>    sceneGraph_;
    u32                         rootNode_;
    u32                         lastNode_;
    u32                         aliveCount_;

    /*********** Components Pools  *************/

    TransformPool               transformPool_;
    GeometryPool                geometryPool_;
    ScriptPool                  scriptPool_;
    MetadataPool                metadataPool_;

    //    LightPool                 light_pool_;

};

template<component T>
decltype(auto) Scene::ComponentPool() noexcept {
    if constexpr (std::is_same<T, Transform>()) {
        return (transformPool_);
    } else if constexpr (std::is_same<T, Script>()) {
        return (scriptPool_);
    } else if constexpr (std::is_same<T, Metadata>()) {
        return (metadataPool_);
    } else {
        return (geometryPool_);
    }
}

extern Scene scene;

template<component T>
T Entity::Component() {
    if (not IsAlive()) return T();
    return scene.ComponentPool<T>().At(id_);
}

template<component T>
T Entity::AddComponent() {
    if (not IsAlive()) return T();
    return scene.ComponentPool<T>().AddComponent(id_);
}

template<component T>
T Entity::AddComponent(T::InitInfo&& initInfo) {
    if (not IsAlive()) return T();
    return scene.ComponentPool<T>().AddComponent(id_, std::forward<T::InitInfo>(initInfo));
}

template<component T>
void Entity::RemoveComponent() {
    if (IsAlive())
        scene.ComponentPool<T>().RemoveComponent(id_);
}

} // reveal3d::core namespace
