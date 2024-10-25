/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file transform.hpp
 * @version 1.0
 * @date 11/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "pool.hpp"

#include <vector>
#include <set>

namespace reveal3d::core {

class Transform {
public:
    struct Info {
        math::xvec3 position    { 0.f, 0.f, 0.f };
        math::xvec3 rotation    { 0.f, 0.f, 0.f };
        math::xvec3 scale       { 1.f, 1.f, 1.f };
    };

    /************* Transform data ****************/
    struct Pool {
        Transform::Info& posRotScale(id_t id)   { return pos_rot_scale.at(id::index(id)); }
        math::mat4& World(id_t id)       { return world_mat.at(id::index(id)); }
        math::mat4& invWorld(id_t id)    { return inv_world.at(id::index(id)); }
        const u32 count()                { return pos_rot_scale.size(); }
        void remove(u32 index)           { }

        utl::vector<math::mat4>         world_mat;
        utl::vector<math::mat4>         inv_world;
        utl::vector<Transform::Info>    pos_rot_scale;
    };

    struct Data {
        Data(math::mat4& world, math::mat4& inv_world, Transform::Info& info)
            : world(world), inv_world(inv_world), info(info) {}
        math::mat4& world;
        math::mat4& inv_world;
        Transform::Info& info;
    };

    using InitInfo = Info;

    Transform() : id_(id::invalid) {}
    explicit Transform(id_t id);

    [[nodiscard]] math::mat4& world() const;
    [[nodiscard]] math::mat4& invWorld() const;
    [[nodiscard]] math::xvec3 position() const;
    [[nodiscard]] math::xvec3 scale() const;
    [[nodiscard]] math::xvec3 rotation() const;
    [[nodiscard]] math::xvec3 worldPosition() const;
    [[nodiscard]] math::xvec3 worldScale() const;
    [[nodiscard]] math::xvec3 worldRotation() const;

    void position(math::xvec3 pos) const;
    void scale(math::xvec3 size) const;
    void rotation(math::xvec3 rot) const;
    void worldPosition(math::xvec3 pos);
    void worldScale(math::xvec3 scale);
    void worldRotation(math::xvec3 rot);
    void update() const;

    [[nodiscard]] inline bool isAlive() const { return id_ != id::invalid; }
    [[nodiscard]] inline id_t id() const { return id_; }

    [[nodiscard]] u8 dirty() const;
    void unDirty() const;
    void setDirty() const;
    Data data();

private:
    static math::mat4 calcWorld(id_t id);
    void updateChilds() const;

    id_t id_ { id::invalid };
};

template<>
inline Transform Pool<Transform>::addComponent(id_t entity_id, Transform::InitInfo &&init_info) {
    id_t transform_id{id_factory_.New(id::index(entity_id))};

    components_data_.pos_rot_scale.push_back(std::move(init_info));
    Transform::Info& data = components_data_.pos_rot_scale.at(count() - 1);
    components_data_.world_mat.emplace_back(math::transpose(math::affine_transformation(data.position, data.scale, data.rotation)));
    components_data_.inv_world.emplace_back(math::inverse(components_data_.world_mat.at(count() - 1)));
    dirties().emplace_back(4);
    dirtyIds().insert(transform_id);

    add(id::index(entity_id), transform_id);

    assert(id::index(transform_id) < components_data_.count());

    return components_ids_.at(id::index(entity_id));
}

template<>
inline Transform Pool<Transform>::addComponent(id_t id) {
    return addComponent(id, {});
}

template<>
inline void Pool<Transform>::removeComponent(id_t id) {
    id_t transform_id {components_ids_.at(id::index(id)).id() };
    if (id_factory_.isAlive(transform_id)) {
        components_data_.pos_rot_scale.unordered_remove(id::index(transform_id));
        components_data_.world_mat.unordered_remove(id::index(transform_id));
        components_data_.inv_world.unordered_remove(id::index(transform_id));
        remove(transform_id);
    }
}

} // reveal3d::core namespace



