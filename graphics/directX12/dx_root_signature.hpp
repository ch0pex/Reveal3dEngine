/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_root_signature.hpp
 * @version 1.0
 * @date 02/04/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "d3dx12.h"
#include "utils/dx_debug.hpp"

namespace reveal3d::graphics::dx12 {

class RootSignature {
public:
  explicit RootSignature(u32 num_root_params = 0);
  void reset(u32 num_root_params);
  void finalize();
  ID3D12RootSignature* get() const { return signature_.Get(); }

  CD3DX12_ROOT_PARAMETER& operator[](size_t entry_index);
  const CD3DX12_ROOT_PARAMETER& operator[](size_t entry_index) const;

private:
  static std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> getStaticSamplers();
  u32 num_parameters_ {0};
  std::unique_ptr<CD3DX12_ROOT_PARAMETER[]> parameters_;
  ComPtr<ID3D12RootSignature> signature_;
};

} // namespace reveal3d::graphics::dx12
