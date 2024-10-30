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

#include "core/scene.hpp"

#include <set>
#include <vector>

namespace reveal3d::core {

class Transform {
public:
    struct Data {
        Data(math::mat4& world, math::mat4& inv_world, transform::Info& info) :
            world(world), inv_world(inv_world), info(info) {}
        math::mat4& world;
        math::mat4& inv_world;
        transform::Info& info;
    };

    using pool_type = transform::Pool;
    using stored_in_gpu = std::true_type;

    constexpr Transform() : id_(id::invalid) {}


    constexpr Transform(id_t id) : id_(id) {}

    [[nodiscard]] constexpr bool isAlive() const { return id_ != id::invalid; }

    [[nodiscard]] constexpr id_t id() const { return id_; }

    [[nodiscard]] math::mat4& world() const { return pool.world(id_); }

    [[nodiscard]] math::mat4& invWorld() const { return pool.invWorld(id_); }

    [[nodiscard]] math::xvec3 position() const { return pool.posRotScale(id_).position; }

    [[nodiscard]] math::xvec3 scale() const { return pool.posRotScale(id_).scale; }

    [[nodiscard]] math::xvec3 rotation() const { return math::vec_to_degrees(pool.posRotScale(id_).rotation); }

    [[nodiscard]] math::xvec3 worldPosition() const {
        math::mat4 world_mat = pool.world(id_);
        return world_mat.translation();
    }

    [[nodiscard]] math::xvec3 worldScale() const { return pool.world(id_).scale(); }

    [[nodiscard]] math::xvec3 worldRotation() const { return pool.world(id_).rotation(); }

    void position(math::xvec3 pos) const {
        pool.posRotScale(id_).position = pos;
        setDirty();
    }

    void scale(math::xvec3 size) const {
        pool.posRotScale(id_).scale = size;
        setDirty();
    }

    void rotation(math::xvec3 rot) const {
        pool.posRotScale(id_).rotation = math::vec_to_radians(rot);
        setDirty();
    }

    void worldPosition(math::xvec3 pos) const {
        transform::Info& trans = pool.posRotScale(id_);
        pool.world(id_) = math::transpose(math::affine_transformation(pos, trans.scale, trans.rotation));
        core::Entity parent = scene.getNode(id_).parent;
        if (parent.isAlive()) {
            trans.position = math::transpose(parent.component<Transform>().invWorld()) * pos;
        }
        else {
            trans.position = pos;
        }

        pool.invWorld(id_) = math::inverse(pool.world(id_));
        pool.dirties().at(id_) = 3;
        pool.dirtyIds().insert(id_);
        updateChildren();
    }

    void worldScale(math::xvec3 scale) const {
        transform::Info& trans = pool.posRotScale(id_);
        pool.world(id_) = math::transpose(math::affine_transformation(trans.position, scale, trans.rotation));
        core::Entity parent = scene.getNode(id_).parent;
        if (parent.isAlive()) {
            trans.scale = parent.component<Transform>().invWorld() * scale;
        }
        else {
            trans.scale = scale;
        }
        pool.invWorld(id_) = math::inverse(pool.world(id_));
        pool.dirties().at(id::index(id_)) = 3;
        pool.dirtyIds().insert(id_);
        updateChildren();
    }

    void worldRotation(math::xvec3 rot) const {
        transform::Info& trans = pool.posRotScale(id_);
        world() = math::transpose(math::affine_transformation(trans.position, trans.scale, rot));
        core::Entity parent = scene.getNode(id_).parent;
        if (parent.isAlive()) {
            trans.rotation = parent.component<Transform>().invWorld() * rot;
        }
        else {
            trans.rotation = rot;
        }
        pool.invWorld(id_) = math::inverse(pool.world(id_));
        pool.dirties().at(id::index(id_)) = 3;
        pool.dirtyIds().insert(id_);
        updateChildren();
    }

    void update() const {
        if (pool.dirties().at(id::index(id_)) != 4) {
            return;
        }

        transform::Info& transform = pool.posRotScale(id_);
        core::Scene::Node& curr_node = scene.getNode(id::index(id_));

        if (curr_node.parent.isAlive()) {
            const id_t parent_id = curr_node.parent.id();
            if (pool.dirties().at(id::index(id_)) < 4) {
                math::mat4 parent_world = pool.world(parent_id);
                world() = parent_world * calcWorld(parent_id);
            }
            else {
                curr_node.parent.component<Transform>().update();
                math::mat4 parent_world = pool.world(parent_id);
                world() = parent_world * calcWorld(id_);
            }
        }
        else {
            world() = calcWorld(id_);
        }
        invWorld() = math::transpose(math::inverse(world()));
        --pool.dirties().at(id::index(id_));
    }

    void unDirty() const {
        const id_t idx = id::index(id_);
        if (pool.dirties().at(idx) != 0) {
            --pool.dirties().at(idx);
        }
        else {
            pool.dirties().at(idx) = 0;
        }
    }

    void setDirty() const {
        if (dirty() == 4) {
            return;
        }
        if (dirty() == 0) {
            pool.dirtyIds().insert(id_);
        }
        updateChildren();
        pool.dirties().at(id::index(id_)) = 4;
    }

    [[nodiscard]] u8 dirty() const { return pool.dirties().at(id::index(id_)); }

    Data data();

private:
    static math::mat4 calcWorld(id_t id) {
        transform::Info& trans = core::scene.componentPool<Transform>().posRotScale(id);
        return math::transpose(math::affine_transformation(trans.position, trans.scale, trans.rotation));
    }

    void updateChildren() const {
        core::Scene::Node& node = scene.getNode(id_);
        auto children = node.getChildren();
        for (auto child_id: children) {
            const Entity child = Entity(child_id);
            child.component<Transform>().setDirty();
            child.component<Transform>().updateChildren();
        }
    }

    inline static GenericPool<transform::Pool>& pool = core::scene.componentPool<Transform>();

    id_t id_{id::invalid};
};

template<>
inline void GenericPool<Transform::pool_type>::update() {
    for (auto it = this->dirty_ids_.begin(); it != this->dirty_ids_.end();) {
        Transform component{*it};
        if constexpr (updatable<Transform>) {
            component.update();
        }
        if (this->dirties_.at(id::index(*it)) == 0) {
            it = this->dirty_ids_.erase(it);
        }
        else {
            ++it;
        }
    }
}

} // namespace reveal3d::core
