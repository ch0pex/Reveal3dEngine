/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file light.hpp
 * @version 1.0
 * @date 14/12/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once


#include "common/common.hpp"
#include "render/light.hpp"
#include "script_pool.hpp"

#include <numeric>


namespace reveal3d::core::light {


class Pool {
public:
  using init_info     = std::pair<render::Light, render::Light::type>;
  using stored_in_gpu = std::true_type;


  render::Light::type lightType(id_t const id) { return light_desc_.at(id::index(id)).second; }

  render::Light& light(id_t const id) {
    auto const [index, type] = light_desc_.at(id::index(id));
    return lights_.at(static_cast<u8>(type)).at(index);
  }

  bool isActive(id_t const id) { return active_.at(id::index(id)); }

  void toggleActive(id_t const id) { active_.at(id::index(id)) = !isActive(id); }

protected:
  [[nodiscard]] u32 countData() {
    auto sizes_vec = lights_ | std::views::transform([](auto const& vec) { return vec.size(); });
    return std::accumulate(sizes_vec.begin(), sizes_vec.end(), 0);
  }

  void addData(id_t entity_id, init_info const& init_info) {
    lights_.at(static_cast<u64>(init_info.second)).push_back(init_info.first);
    active_.at(static_cast<u64>(init_info.second));
    light_desc_.emplace_back(lights_.at(static_cast<u64>(init_info.second)).size(), init_info.second);
  }

  void removeData(u32 const id) {
    auto [index, type] = light_desc_.at(id::index(id));
    lights_.at(static_cast<u64>(type)).unordered_remove(index);
    active_.unordered_remove(id::index(id));
    light_desc_.unordered_remove(id::index(id));
  }

private:
  std::array<utl::vector<render::Light>, static_cast<u64>(render::Light::type::count)> lights_;
  utl::vector<std::pair<index_t, render::Light::type>> light_desc_;
  utl::vector<bool> active_;
};

} // namespace reveal3d::core::light
