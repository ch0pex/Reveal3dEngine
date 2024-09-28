/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file light.cpp
 * @version 1.0
 * @date 11/03/2024
 * @brief Short description
 *
 * Longer description
 */

#include "light.hpp"

#include <vector>

namespace reveal3d::render {

namespace  {

struct LightData {
    math::vec3 color;       // Light color and strength
    math::vec3 direction;   // Spot and directional only
    math::vec3 position;    // Spot and point only
    f32 fallOffStart;       // Spot and point only
    f32 fallOffEnd;         // Spot and point only
    f32 spotPower;          // Spot only
};



std::vector<LightData> lights;

} // Anonymous namespace

Light::Light(id_t id) : id_ { id } {
   id_t index { id::index(id) };


}

void Light::IsEnabled(bool is_enabled) const {

}

void Light::Itensity(f32 intensity) const {

}

void Light::Color(math::vec3 color) const {

}

void Light::Attenuation(math::vec3 attenuation) const {

}

void Light::Range(f32 range) const {

}

void Light::ConeAngles(f32 umbra, f32 penumbra) const {

}

bool Light::IsEnabled() const {
    return false;
}

f32 Light::Intensity() const {
    return 0;
}

math::vec3 Light::Color() const {
    return reveal3d::math::vec3();
}

math::vec3 Light::Attenuation() const {
    return reveal3d::math::vec3();
}

f32 Light::Range() const {
    return 0;
}

f32 Light::Umbra() const { return 0; }
f32 Light::Penumbra() const { return 0; }
Light::type Light::Type() const { return Light::point; }
id_t Light::Id() const { return 0; }


}