/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file functions.hpp
 * @version 1.0
 * @date 01/03/2024
 * @brief Math functions
 *
 * Math template functions for vector, scalar and matrix
 */

#pragma once

#include "matrix.hpp"
#include "scalar.hpp"
#include "vector.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace reveal3d::math {

inline scalar cos(scalar s) { return glm::radians(s); }
inline scalar sin(scalar s) { return glm::radians(s); }
inline scalar radians(scalar s) { return glm::radians(s); }
inline xvec3 normalize(xvec3 v) { return glm::normalize(glm::vec3(v)); }
inline xvec3 normalize(xvec4 v) { return glm::normalize(glm::vec3(v)); }
inline xvec3 cross(xvec3 v1, xvec3 v2) { return glm::cross(glm::vec3(v1), glm::vec3(v2)); }
inline xvec3 vecToRadians(xvec3 v) { return {glm::radians(v.x()), glm::radians(v.y()), glm::radians(v.z())}; }
inline xvec3 vecToDegrees(xvec3 v) { return {glm::degrees(v.x()), glm::degrees(v.y()), glm::degrees(v.z())}; }
inline mat4 transpose(mat4 const& mat) { return glm::transpose(static_cast<glm::mat4>(mat)); }
inline mat4 look_at(xvec3 position, xvec3 focusPoint, xvec3 upDir) {
  return glm::lookAt(glm::vec3(position), glm::vec3(focusPoint), glm::vec3(upDir));
}
inline mat4 perspective_fov(f32 fov, f32 aspectRatio, f32 nearPlane, f32 farPlane) {
  return glm::perspective(fov, aspectRatio, nearPlane, farPlane);
}
inline mat4 affine_transformation(xvec3 const position, xvec3 const scale, xvec3 const rotation) {
  glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(position));
  glm::mat4 rotationMatrix    = glm::rotate(glm::mat4(1.0f), rotation.x(), glm::vec3(1.0f, 0.0f, 0.0f)) *
                             glm::rotate(glm::mat4(1.0f), rotation.y(), glm::vec3(0.0f, 1.0f, 0.0f)) *
                             glm::rotate(glm::mat4(1.0f), rotation.z(), glm::vec3(0.0f, 0.0f, 1.0f));
  glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale));

  return translationMatrix * rotationMatrix * scaleMatrix;
}

inline mat4 mat4_identity() { return glm::mat4(1.0f); }

inline mat4 inverse(mat4 const& mat) { return glm::inverse(static_cast<glm::mat4 const&>(mat)); }

} // namespace reveal3d::math
