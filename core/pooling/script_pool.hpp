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

#include <functional>

namespace reveal3d::core::script {

class ScriptBase {
  std::function<void()> begin;
  std::function<void()> update;
  std::function<void()> destroyed;
};

struct Flag {
  u8 begin : 1; // begin function active
  u8 update : 1; // update function active
  u8 destroy : 1; // destroy function active
};

struct Data {
  Flag flags;
  std::unique_ptr<ScriptBase> scripts;
};

class Pool {
public:
  using init_info  = std::string;
  using gpu_stored = std::false_type;

  ScriptBase* scripts(id_t const id) { return scripts_.at(id::index(id)).get(); }

protected:
  [[nodiscard]] u32 countData() const { return scripts_.size(); }

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
  utl::vector<Flag> flags_;
  utl::vector<std::unique_ptr<ScriptBase>> scripts_;
};

} // namespace reveal3d::core::script
