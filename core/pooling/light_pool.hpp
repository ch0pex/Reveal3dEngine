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
  using init_info  = std::pair<render::Light, render::Light::type>;
  using gpu_stored = std::true_type;


  render::Light::type lightType(id_t const id) { return light_desc_.at(id::index(id)).second; }

  render::Light& light(id_t const id) {
    auto const [index, type] = light_desc_.at(id::index(id));
    return lights_.at(static_cast<u8>(type)).at(index);
  }

  bool isActive(id_t const id) { return light(id).active; }

  void toggleActive(id_t const id) { light(id).active = !light(id).active; }

protected:
  [[nodiscard]] index_t countData() {
    auto sizes_vec {lights_ | std::views::transform([](auto const& vec) { return vec.size(); })};
    return static_cast<index_t>(std::accumulate(sizes_vec.begin(), sizes_vec.end(), u64 {0}));
  }

  void addData(init_info const& init_info) {
    light_desc_.emplace_back(
        static_cast<index_t>(lights_.at(static_cast<index_t>(init_info.second)).size()), init_info.second
    );
    lights_.at(static_cast<index_t>(init_info.second)).push_back(init_info.first);
  }

  void removeData(id_t const id) {
    auto [index, type] = light_desc_.at(id::index(id));
    lights_.at(static_cast<index_t>(type)).unordered_remove(index);
    light_desc_.unordered_remove(id::index(id));
  }

private:
  std::array<utils::vector<render::Light>, static_cast<u64>(render::Light::type::count)> lights_;
  utils::vector<std::pair<index_t, render::Light::type>> light_desc_;
};

} // namespace reveal3d::core::light
