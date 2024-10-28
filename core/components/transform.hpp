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
        math::xvec3 position { 0.f, 0.f, 0.f };
        math::xvec3 rotation { 0.f, 0.f, 0.f };
        math::xvec3 scale    { 1.f, 1.f, 1.f };
    };

    using InitInfo = Info;

    /************* Transform data ****************/
    class Pool {
    public:
        Transform::Info& posRotScale(id_t id) { return pos_rot_scale_.at(id::index(id)); }

        math::mat4& world(id_t id) { return world_mat_.at(id::index(id)); }

        math::mat4& invWorld(id_t id) { return inv_world_.at(id::index(id)); }

        const u32 count() { return pos_rot_scale_.size(); }

        void add(id_t entity_id, InitInfo &init_info) {
            Transform::Info& data = pos_rot_scale_.at(count() - 1);
            pos_rot_scale_.push_back(std::move(init_info));
            world_mat_.emplace_back(math::transpose(math::affine_transformation(data.position, data.scale, data.rotation)));
            inv_world_.emplace_back(math::inverse(world_mat_.at(count() - 1)));
        }

        void remove(u32 id) {
            pos_rot_scale_.unordered_remove(id::index(id));
            world_mat_.unordered_remove(id::index(id));
            inv_world_.unordered_remove(id::index(id));
        }

    private:
        utl::vector<math::mat4> world_mat_;
        utl::vector<math::mat4> inv_world_;
        utl::vector<Transform::Info> pos_rot_scale_;
    };

    struct Data {
        Data(math::mat4& world, math::mat4& inv_world, Transform::Info& info)
            : world(world), inv_world(inv_world), info(info) {}
        math::mat4& world;
        math::mat4& inv_world;
        Transform::Info& info;
    };


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
    void worldPosition(math::xvec3 pos) const;
    void worldScale(math::xvec3 scale) const;
    void worldRotation(math::xvec3 rot) const;
    void update() const;

    [[nodiscard]] inline bool isAlive() const { return id_ != id::invalid; }
    [[nodiscard]] inline id_t id() const { return id_; }

    [[nodiscard]] u8 dirty() const;
    void unDirty() const;
    void setDirty() const;
    Data data();

private:
    static math::mat4 calcWorld(id_t id);
    void updateChildren() const;

    id_t id_ { id::invalid };
};


} // reveal3d::core namespace



