/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file transform_pool.hpp
 * @version 1.0
 * @date 29/10/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

namespace reveal3d::core::metadata {

class Pool {
public:
  using init_info     = std::string;
  using stored_in_gpu = std::false_type;

  std::string& name(id_t const id) { return names_.at(id::index(id)); }

  std::string& comment(id_t const id) { return comments_.at(id::index(id)); }

  std::string& date(id_t const id) { return dates_.at(id::index(id)); }

  u32 countData() const { return names_.size(); }

  void addData(id_t const entity_id, init_info const& init_info) {
    names_.emplace_back("Entity_" + std::to_string(entity_id));
    comments_.emplace_back();
    dates_.emplace_back("10/12/2024"); // TODO
    comments_.at(comments_.size() - 1U).reserve(1024);
  }

  void removeData(u32 const id) {
    names_.unordered_remove(id::index(id));
    comments_.unordered_remove(id::index(id));
    dates_.unordered_remove(id::index(id));
  }

private:
  utl::vector<std::string> names_;
  utl::vector<std::string> comments_;
  utl::vector<std::string> dates_;
};


} // namespace reveal3d::core::metadata
