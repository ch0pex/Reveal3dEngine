/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file string_literals.hpp
 * @version 1.0
 * @date 18/10/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include <algorithm>
#include <codecvt>
#include <filesystem>
#include <locale>
#include <source_location>
#include <string>

#include "primitive_types.hpp"

namespace reveal3d {

std::filesystem::path const reveal3d_path =
    std::filesystem::path(std::source_location::current().file_name()).parent_path() / "../";


inline std::wstring absolute(std::wstring_view const path) { return (reveal3d_path / path).wstring(); }

inline std::string absolute(std::string_view const path) { return (reveal3d_path / path).string(); }

namespace literals {

template<size_t N>
struct String {

  constexpr String(char const (&str)[N]) { std::ranges::copy_n(str, N, value); }

  operator std::string() const { return std::string(value); }

  char value[N];
};

inline auto operator""_abs(char const* string, u64) -> std::string { return absolute(string); }

inline auto operator""_abs(wchar_t const* string, u64) -> std::wstring { return absolute(string); }

// inline auto operator""_sl(char const (&str)[256]) { return String {str}; }

} // namespace literals

} // namespace reveal3d
