/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_checker.hpp
 * @version 1.0
 * @date 04/11/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "common/common.hpp"

#include <d3d12.h>
#include <fmt/core.h>
#include <source_location>
#include <stdexcept>
#include <string>
#include <windows.h>
#include <wrl/client.h>

using namespace DirectX;
using Microsoft::WRL::ComPtr;

namespace reveal3d::graphics::dx12::utils {


struct Checker { };

inline Checker DxCheck;

struct Error {
  Error(u32 hr, const std::source_location& loc = std::source_location::current()) noexcept : hr(hr), loc(loc) {};
  u32 hr;
  std::source_location loc;
};

inline void operator>>(Error grabber, Checker checker) {
  if (FAILED(grabber.hr)) {
    const std::string error = fmt::format(
        "{}:{}:{}: HRESULT failed with error code {}", grabber.loc.file_name(), grabber.loc.line(),
        grabber.loc.column(), grabber.hr
    );
    logger(LogInfo) << error;
    MessageBoxA(nullptr, error.c_str(), "Error details", MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2);
    throw std::runtime_error(error);
  }
}


}; // namespace reveal3d::graphics::dx12::utl
