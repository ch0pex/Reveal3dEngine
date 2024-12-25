/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_resource_array.hpp
 * @version 1.0
 * @date 26/12/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "common/common.hpp"
#include "config/config.hpp"

#include <cassert>

namespace reveal3d::graphics::dx12::utl {

/**
 * @class ResourceArray
 * @brief This class offers a safe array for that resources that need to be replicated as times as swap chain buffers
 * @tparam T Resource type
 */
template<typename T>
class ResourceArray {
public:
  using value_type     = T;
  using container      = std::array<value_type, config::Render::Graphics::max_buffer_count>;
  using iterator       = typename container::iterator;
  using const_iterator = typename container::const_iterator;

  [[nodiscard]] constexpr const_iterator begin() const { return const_iterator(resource_.begin()); }

  [[nodiscard]] constexpr const_iterator end() const {
    return const_iterator(resource_.begin() + config::render.graphics.buffer_count);
  }

  constexpr iterator begin() { return iterator(resource_.begin()); }

  constexpr iterator end() { return iterator(resource_.begin() + config::render.graphics.buffer_count); }

  constexpr T& at(u8 const index) {
    assert(index < config::render.graphics.buffer_count);
    return resource_.at(index);
  }

private:
  container resource_;
};

} // namespace reveal3d::graphics::dx12::utl
