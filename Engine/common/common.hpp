/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details) 
 ************************************************************************/
/**
 * @file common.h
 * @version 1.0
 * @date 26/02/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "primitive_types.hpp"
#include "logger.hpp"
#include "platform.hpp"

namespace reveal3d {

constexpr wchar_t PROJECT_DIR_L[] = L"" PROJECT_ROOT_DIR;
constexpr char PROJECT_DIR[] = "" PROJECT_ROOT_DIR;

INLINE std::wstring relative(const wchar_t* wstring) {
    const std::wstring p = PROJECT_DIR_L;
    const std::wstring s = wstring;
    return (p + L"/" + s);
}

INLINE std::string relative(const char* string) {
    const std::string p = PROJECT_DIR;
    const std::string s = string;
    return (p + "/" + s);
}

}

