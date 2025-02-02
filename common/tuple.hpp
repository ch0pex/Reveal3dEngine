/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file tuple.hpp
 * @version 1.0
 * @date 27/01/2025
 * @brief Short description
 *
 * Longer description
 */

#pragma once

namespace reveal3d::tuple {


template<std::size_t... Idx>
auto make_index_dispatcher(std::index_sequence<Idx...>) {
  return [](auto&& f) { (f(std::integral_constant<std::size_t, Idx> {}), ...); };
}

template<std::size_t N>
auto make_index_dispatcher() {
  return make_index_dispatcher(std::make_index_sequence<N> {});
}

template<typename Tuple, typename Func>
void for_each(Tuple&& t, Func&& f) {
  constexpr auto n = std::tuple_size_v<std::decay_t<Tuple>>;
  auto dispatcher  = make_index_dispatcher<n>();
  dispatcher([&f, &t](auto idx) { f(std::get<idx>(std::forward<Tuple>(t))); });
}

template<std::size_t... Idx>
auto transform_index_dispatcher(std::index_sequence<Idx...>) {
  return [](auto&& f) { std::make_tuple(f(std::integral_constant<std::size_t, Idx> {})...); };
}

template<std::size_t N>
auto transform_index_dispatcher() {
  return transform_index_dispatcher(std::make_index_sequence<N> {});
}

template<typename Tuple, typename Func>
auto transform(Tuple&& t, Func&& f) {
  constexpr auto n = std::tuple_size_v<std::decay_t<Tuple>>;
  auto dispatcher  = make_index_dispatcher<n>();
  dispatcher([&f, &t](auto idx) { f(std::get<idx>(std::forward<Tuple>(t))); });
}

} // namespace reveal3d::tuple
