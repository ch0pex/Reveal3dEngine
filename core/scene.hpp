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
 * ************************************************ Component pool *****************************************************
 * ************************** Scene IDs ********************************************* Scene Data ***********************
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

#include "common/id.hpp"
#include "common/timer.hpp"
#include "content/primitives.hpp"
#include "geometry.hpp"
#include "render/light.hpp"
#include "script.hpp"
#include "transform.hpp"

#include <deque>
#include <set>
#include <vector>


namespace reveal3d::core {

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

    Entity NewEntity();
    Entity NewChildEntity(Entity parent);
    bool   RemoveEntity(id_t id);
    bool   IsEntityAlive(id_t id);

    /******************************* Scene Graph methods ******************************/
    [[nodiscard]] INLINE const std::vector<Scene::Node>& Graph() const { return sceneGraph_; }
    [[nodiscard]] INLINE u32 NumEntities() const { return sceneGraph_.size(); }
    INLINE Entity GetEntity(id_t id) { return sceneGraph_.at(id::index(id)).entity; }
    INLINE Node& GetNode(id_t id) { return sceneGraph_.at(id::index(id)); }
    INLINE Node& Root() { return sceneGraph_.at(0); }

    // NOTE: references, yes this is not the best TODO fix this
//    std::vector<Transform>& Transforms();
//    std::vector<Geometry>& Geometries();

    template<typename T> auto& ComponentPool();

//    std::set<id_t>& DirtyTransforms();
//    std::set<id_t>& DirtyGeometries();
//    std::set<id_t>& DirtyLights();

    void Init();
    void Update(f32 dt);

private:

    /************ Entity IDs factory ***********/

    id_t NewId();

    std::vector<id_t>           generations_;
    std::deque<id_t>            freeIndices_;

    /************** Entity graph****************/

    Node*                       lastNode_;
    std::vector<Scene::Node>    sceneGraph_;

    /*********** Components Pools  *************/

    TransformPool               transform_pool_;
    GeometryPool                geometry_pool_;

//    MetadataPool              metadata_pool_;
//    ScriptPool                script_pool_;
//    LightPool                 light_pool_;

};

extern Scene scene;

template<typename T>
auto& Scene::ComponentPool() {
    if constexpr (std::is_same<T, Transform>()) {
        return transform_pool_;
    } else if constexpr (std::is_same<T, Geometry>()){
        return geometry_pool_;
    }
}

template<typename T>
T Entity::Component() {
    return scene.ComponentPool<T>().At(id_);
}

template<typename T>
void Entity::AddComponent() {
    scene.ComponentPool<T>().AddComponent(id_);
}

template<typename T>
void Entity::AddComponent(T&& data) {
    scene.ComponentPool<T>().AddComponent(id_, std::forward(data));
}

}
