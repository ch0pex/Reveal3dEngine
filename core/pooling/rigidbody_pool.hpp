/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file rigidbody_pool.hpp
 * @version 1.0
 * @date 31/10/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "common/common.hpp"
#include "concepts.hpp"

namespace reveal3d::core::rigidbody {

class Pool {
public:
  using init_info  = std::string;
  using gpu_stored = std::false_type;

  [[nodiscard]] index_t countData() const { return 0; }

  void addData(init_info const& init_info) {
    //        names_.emplace_back("Entity_" + std::to_string(entity_id));
    //        comments_.emplace_back();
    //        dates_.emplace_back("10/12/2024");  //TODO
    //        comments_.at(comments_.size() - 1U).reserve(1024);
  }

  void removeData(u32 id) {
    //        names_.unordered_remove(id::index(id));
    //        comments_.unordered_remove(id::index(id));
    //        dates_.unordered_remove(id::index(id));
  }

private:
};

static_assert(pool_impl<Pool>);

} // namespace reveal3d::core::rigidbody
