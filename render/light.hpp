/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file light.hpp
 * @version 1.0
 * @date 11/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "common/common.hpp"
#include "vector.hpp"


namespace reveal3d::render {

class Light {
public:
    enum type {
        directional,
        point,
        spot,

        count
    };

    explicit Light(id_t id);
    constexpr Light() = default;
    [[nodiscard]] constexpr bool IsValid() const { return id::isValid(id_); }

    void IsEnabled(bool is_enabled) const;
    void Itensity(f32 intensity) const;
    void Color(math::vec3 color) const;
    void Attenuation(math::vec3 attenuation) const;
    void Range(f32 range) const;
    void ConeAngles(f32 umbra, f32 penumbra) const;

    [[nodiscard]] bool IsEnabled() const;
    [[nodiscard]] f32 Intensity() const;
    [[nodiscard]] math::vec3 Color() const;
    [[nodiscard]] math::vec3 Attenuation() const;
    [[nodiscard]] f32 Range() const;
    [[nodiscard]] f32 Umbra() const;
    [[nodiscard]] f32 Penumbra() const;
    [[nodiscard]] type Type() const;
    [[nodiscard]] id_t Id() const;

private:
    id_t id_ { id::invalid };
};

}