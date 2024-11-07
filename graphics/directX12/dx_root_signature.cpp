/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_root_signature.cpp
 * @version 1.0
 * @date 02/04/2024
 * @brief Short description
 *
 * Longer description
 */

#include "dx_root_signature.hpp"

#include <array>

namespace reveal3d::graphics::dx12 {
/*

RootParameter::RootParameter() {
    parameter_.ParameterType = (D3D12_ROOT_PARAMETER_TYPE) 0xFFFFFFFF;
}


RootParameter::~RootParameter()
{
    Clear();
}

void RootParameter::clear()
{
    if (parameter_.ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
        delete [] parameter_.DescriptorTable.pDescriptorRanges;

    parameter_.ParameterType = (D3D12_ROOT_PARAMETER_TYPE)0xFFFFFFFF;
}

void RootParameter::InitAsConstants( UINT Register, UINT NumDwords, D3D12_SHADER_VISIBILITY Visibility =
D3D12_SHADER_VISIBILITY_ALL, UINT Space = 0 )
{
    parameter_.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
    parameter_.ShaderVisibility = Visibility;
    parameter_.Constants.Num32BitValues = NumDwords;
    parameter_.Constants.ShaderRegister = Register;
    parameter_.Constants.RegisterSpace = Space;
}

void RootParameter::InitAsConstantBuffer( UINT Register, D3D12_SHADER_VISIBILITY Visibility =
D3D12_SHADER_VISIBILITY_ALL, UINT Space = 0 )
{
    parameter_.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
    parameter_.ShaderVisibility = Visibility;
    parameter_.Descriptor.ShaderRegister = Register;
    parameter_.Descriptor.RegisterSpace = Space;
}

void RootParameter::InitAsBufferSRV( UINT Register, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL,
UINT Space = 0 )
{
    parameter_.ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
    parameter_.ShaderVisibility = Visibility;
    parameter_.Descriptor.ShaderRegister = Register;
    parameter_.Descriptor.RegisterSpace = Space;
}

void RootParameter::InitAsBufferUAV( UINT Register, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL,
UINT Space = 0 )
{
    parameter_.ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
    parameter_.ShaderVisibility = Visibility;
    parameter_.Descriptor.ShaderRegister = Register;
    parameter_.Descriptor.RegisterSpace = Space;
}

void RootParameter::InitAsDescriptorRange( D3D12_DESCRIPTOR_RANGE_TYPE Type, UINT Register, UINT count,
D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL, UINT Space = 0 )
{
    InitAsDescriptorTable(1, Visibility);
    SetTableRange(0, Type, Register, count, Space);
}

void RootParameter::InitAsDescriptorTable( UINT RangeCount, D3D12_SHADER_VISIBILITY Visibility =
D3D12_SHADER_VISIBILITY_ALL )
{
    parameter_.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    parameter_.ShaderVisibility = Visibility;
    parameter_.DescriptorTable.NumDescriptorRanges = RangeCount;
    parameter_.DescriptorTable.pDescriptorRanges = new D3D12_DESCRIPTOR_RANGE[RangeCount];
}

void RootParameter::SetTableRange( UINT RangeIndex, D3D12_DESCRIPTOR_RANGE_TYPE Type, UINT Register, UINT count, UINT
Space = 0 )
{
    D3D12_DESCRIPTOR_RANGE* range = const_cast<D3D12_DESCRIPTOR_RANGE*>(parameter_.DescriptorTable.pDescriptorRanges +
RangeIndex); range->RangeType = Type; range->NumDescriptors = countData; range->BaseShaderRegister = Register;
    range->RegisterSpace = Space;
    range->OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
}
*/

RootSignature::RootSignature(u32 num_root_params) : num_parameters_(num_root_params) { reset(num_root_params); }

void RootSignature::reset(u32 num_root_params) {
  if (num_root_params > 0) {
    parameters_.reset(new CD3DX12_ROOT_PARAMETER[num_root_params]);
  }
  else {
    parameters_ = nullptr;
  }
  num_parameters_ = num_root_params;
}

void RootSignature::finalize(ID3D12Device* device) {
  auto static_samplers = getStaticSamplers();
  CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(
      num_parameters_, parameters_.get(), static_samplers.size(), static_samplers.data(),
      D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
  );

  // create a root signature with a single slot which points to a descriptor range consisting of a single constant
  // buffer
  ComPtr<ID3DBlob> serialized_root_sig = nullptr;
  ComPtr<ID3DBlob> error_blob          = nullptr;
  const HRESULT hr                     = D3D12SerializeRootSignature(
      &rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, serialized_root_sig.GetAddressOf(), error_blob.GetAddressOf()
  );

  if (error_blob != nullptr) {
    OutputDebugStringA((char*)error_blob->GetBufferPointer());
  }
  hr >> utl::DxCheck;

  device->CreateRootSignature(
      0, serialized_root_sig->GetBufferPointer(), serialized_root_sig->GetBufferSize(),
      IID_PPV_ARGS(signature_.GetAddressOf())
  ) >> utl::DxCheck;
}

const CD3DX12_ROOT_PARAMETER& RootSignature::operator[](size_t entry_index) const {
  assert(entry_index < num_parameters_);
  return parameters_.get()[entry_index];
}

CD3DX12_ROOT_PARAMETER& RootSignature::operator[](size_t entry_index) {
  assert(entry_index < num_parameters_);
  return parameters_.get()[entry_index];
}

std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> RootSignature::getStaticSamplers() {
  // Applications usually only need a handful of samplers.  So just define them all up front
  // and keep them available as part of the root signature.

  const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
      0, // shaderRegister
      D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
      D3D12_TEXTURE_ADDRESS_MODE_WRAP, // addressU
      D3D12_TEXTURE_ADDRESS_MODE_WRAP, // addressV
      D3D12_TEXTURE_ADDRESS_MODE_WRAP
  ); // addressW

  const CD3DX12_STATIC_SAMPLER_DESC pointClamp(
      1, // shaderRegister
      D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
      D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // addressU
      D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // addressV
      D3D12_TEXTURE_ADDRESS_MODE_CLAMP
  ); // addressW

  const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
      2, // shaderRegister
      D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
      D3D12_TEXTURE_ADDRESS_MODE_WRAP, // addressU
      D3D12_TEXTURE_ADDRESS_MODE_WRAP, // addressV
      D3D12_TEXTURE_ADDRESS_MODE_WRAP
  ); // addressW

  const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
      3, // shaderRegister
      D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
      D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // addressU
      D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // addressV
      D3D12_TEXTURE_ADDRESS_MODE_CLAMP
  ); // addressW

  const CD3DX12_STATIC_SAMPLER_DESC anisotropicWrap(
      4, // shaderRegister
      D3D12_FILTER_ANISOTROPIC, // filter
      D3D12_TEXTURE_ADDRESS_MODE_WRAP, // addressU
      D3D12_TEXTURE_ADDRESS_MODE_WRAP, // addressV
      D3D12_TEXTURE_ADDRESS_MODE_WRAP, // addressW
      0.0f, // mipLODBias
      8
  ); // maxAnisotropy

  const CD3DX12_STATIC_SAMPLER_DESC anisotropicClamp(
      5, // shaderRegister
      D3D12_FILTER_ANISOTROPIC, // filter
      D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // addressU
      D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // addressV
      D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // addressW
      0.0f, // mipLODBias
      8
  ); // maxAnisotropy

  return {pointWrap, pointClamp, linearWrap, linearClamp, anisotropicWrap, anisotropicClamp};
}


} // namespace reveal3d::graphics::dx12
