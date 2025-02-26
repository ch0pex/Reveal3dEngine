/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_resources.hpp
 * @version 1.0
 * @date 25/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "core/components/transform.hpp"
#include "dx_upload_buffer.hpp"
#include "graphics/constants.hpp"
#include "render/camera.hpp"
#include "render/material.hpp"

namespace reveal3d::graphics::dx12 {

struct FrameResource {
  void update(core::Scene& scene, render::Camera const& camera) {
    auto& geometries = scene.pool<core::Geometry>();

    auto const view_proj = transpose(camera.getViewProjectionMatrix());
    pass_buffer.at(0)    = {
         .view          = camera.getViewMatrix(),
         .inv_view      = inverse(camera.getViewMatrix()),
         .proj          = camera.getProjectionMatrix(),
         .inv_proj      = inverse(camera.getProjectionMatrix()),
         .view_proj     = view_proj,
         .inv_view_proj = inverse(view_proj),
         .eye_pos       = camera.position(),
         .near_z        = camera.nearPlane(),
         .far_z         = camera.farPlane(),
    };

    // update object constants
    for (auto const id: scene.pool<core::Transform>().dirtyElements()) {
      core::Transform const trans {&scene, id};
      per_obj_buffer.at(id::index(id)) = {
        .world_view_proj = trans.world(), .entity_id = trans.entity() //
      };
      trans.unDirty();
    }

    // update material constants
    for (auto const id: geometries.dirtyElements()) {
      core::Geometry const geo {&scene, id};
      mat_buffer.at(id::index(id)) = geo.material();
      geo.unDirty();
    }
  }

  ConstantBuffer<PerObjectData> per_obj_buffer {100'000};
  ConstantBuffer<GlobalShaderData> pass_buffer {1};
  ConstantBuffer<render::Material> mat_buffer {100'000};
};

} // namespace reveal3d::graphics::dx12
