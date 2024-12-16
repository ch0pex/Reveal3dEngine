/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file geometry.hpp
 * @version 1.0
 * @date 11/03/2024
 * @brief Short description
 *
 * Geometry entity component
 */

#pragma once

#include "component.hpp"

#include <string>


namespace reveal3d::core {

struct Metadata : Component<Metadata> {
  using pool_type = metadata::Pool;
  using init_info = std::string;
  using Component::Component;

  [[nodiscard]] std::string& name() const { return pool().name(id_); }

  [[nodiscard]] std::string& date() const { return pool().date(id_); }

  [[nodiscard]] std::string& comment() const { return pool().comment(id_); }

private:
  static auto constexpr pool = []() -> GenericPool<pool_type>& { return scene.componentPool<Metadata>(); };
};

} // namespace reveal3d::core
