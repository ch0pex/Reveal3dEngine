/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_shader.hpp
 * @version 1.0
 * @date 16/01/2025
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "common/string_literals.hpp"
#include "utils/dx_checker.hpp"

#include <d3dcompiler.h>

using Microsoft::WRL::ComPtr;

namespace reveal3d::graphics::dx12 {

struct ShaderBinary {
  ComPtr<ID3DBlob> vs;
  ComPtr<ID3DBlob> ps;
};

inline ShaderBinary compile_shader(std::wstring_view const vertex_file, std::wstring_view const pixel_file) {
  ComPtr<ID3DBlob> vertex_shader;
  ComPtr<ID3DBlob> pixel_shader;
  ComPtr<ID3DBlob> errors;

#if defined(_DEBUG)
  // Enable better Shader debugging with the graphics debugging tools.
  UINT compile_flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
  UINT compile_flags = 0;
#endif
  HRESULT hr = S_OK;

  // TODO Config file for assets path
  hr = D3DCompileFromFile(
      absolute(vertex_file).c_str(), nullptr, nullptr, "VS", "vs_5_0", compile_flags, 0, &vertex_shader, &errors
  );
  if (errors != nullptr)
    logger(LogInfo) << static_cast<char*>(errors->GetBufferPointer());

  hr >> utils::DxCheck;
  hr = D3DCompileFromFile(
      absolute(pixel_file).c_str(), nullptr, nullptr, "PS", "ps_5_0", compile_flags, 0, &pixel_shader, &errors
  );
  if (errors != nullptr)
    logger(LogInfo) << static_cast<char*>(errors->GetBufferPointer());
  hr >> utils::DxCheck;

  return {vertex_shader, pixel_shader};
}

} // namespace reveal3d::graphics::dx12
