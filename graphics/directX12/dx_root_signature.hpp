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

#include "dx_common.hpp"

namespace reveal3d::graphics::dx12 {

/*
class RootParameter {
public:
    RootParameter();

        ~RootParameter()
        {
            Clear();
        }

        void clear();
        void InitAsConstants( u32 reg, u32 words, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL, u32
Space = 0 ); void InitAsConstantBuffer( u32 reg, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL, u32
Space = 0 ); void InitAsBufferSRV( u32 reg, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL, u32 Space
= 0 ); void InitAsBufferUAV( u32 reg, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL, u32 Space = 0 );
        void InitAsDescriptorRange( D3D12_DESCRIPTOR_RANGE_TYPE type, u32 reg, u32 count, D3D12_SHADER_VISIBILITY
visibility = D3D12_SHADER_VISIBILITY_ALL, u32 spcae = 0 ); void InitAsDescriptorTable( u32 RangeCount,
D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL ); void SetTableRange( u32 rangeIndex,
D3D12_DESCRIPTOR_RANGE_TYPE type, u32 reg, u32 countData, u32 space = 0 );

        const D3D12_ROOT_PARAMETER& operator() ( void ) const { return parameter_; }

private:
    D3D12_ROOT_PARAMETER parameter_;
};
*/

class RootSignature {
public:
  RootSignature(u32 num_root_params = 0);
  void reset(u32 num_root_params);
  void finalize(ID3D12Device* device);
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
