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
#include <string>
#include <codecvt>
#include <locale>

namespace reveal3d {

template<size_t N>
struct StringLiteral {

    constexpr StringLiteral(const char (&str)[N]) {
        std::copy_n(str, N, value);
    }

    operator std::string() const {
        return std::string(value);
    }

    char value[N];
};


constexpr StringLiteral project_dir {PROJECT_ROOT_DIR};
constexpr wchar_t project_dir_l[] = L"" PROJECT_ROOT_DIR;


inline std::wstring relative(const std::wstring_view path) {
    const std::wstring p = project_dir_l;
    const std::wstring s { path };
    return (p + L"/Source/Reveal3d/" + s); //TODO: fix relative paths
}

inline std::string relative(const std::string_view path) {
    const std::string p = project_dir;
    const std::string s { path };
    return (p + "/Source/Reveal3d/" + s); //TODO: fix relative paths
}

}
