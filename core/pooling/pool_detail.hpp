/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file pool_detail.hpp
 * @version 1.0
 * @date 27/01/2025
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include <queue>
#include <set>

namespace reveal3d::core::detail {

template<typename InGPU>
class GpuSynchronize { };

template<>
class GpuSynchronize<std::true_type> {
public:
  id_t popNew() {
    if (new_components_.empty()) {
      return id::invalid;
    }
    auto const id = new_components_.front();
    new_components_.pop();
    return id;
  }
  id_t popRemoved() {
    if (deleted_components_.empty()) {
      return id::invalid;
    }
    auto const id = deleted_components_.front();
    deleted_components_.pop();
    return id;
  }

  std::set<id_t>& dirtyElements() { return dirty_ids_; }

  std::set<id_t>& dirtyIds() { return dirty_ids_; }

  utils::vector<u8>& dirties() { return dirties_; }

protected:
  std::queue<id_t> new_components_;
  std::queue<id_t> deleted_components_;
  std::set<id_t> dirty_ids_;
  utils::vector<u8> dirties_;
};

} // namespace reveal3d::core::detail
