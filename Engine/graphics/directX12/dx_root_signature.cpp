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

namespace reveal3d::graphics::dx {
/*

RootParameter::RootParameter() {
    parameter_.ParameterType = (D3D12_ROOT_PARAMETER_TYPE) 0xFFFFFFFF;
}


RootParameter::~RootParameter()
{
    Clear();
}

void RootParameter::Clear()
{
    if (parameter_.ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
        delete [] parameter_.DescriptorTable.pDescriptorRanges;

    parameter_.ParameterType = (D3D12_ROOT_PARAMETER_TYPE)0xFFFFFFFF;
}

void RootParameter::InitAsConstants( UINT Register, UINT NumDwords, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL, UINT Space = 0 )
{
    parameter_.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
    parameter_.ShaderVisibility = Visibility;
    parameter_.Constants.Num32BitValues = NumDwords;
    parameter_.Constants.ShaderRegister = Register;
    parameter_.Constants.RegisterSpace = Space;
}

void RootParameter::InitAsConstantBuffer( UINT Register, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL, UINT Space = 0 )
{
    parameter_.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
    parameter_.ShaderVisibility = Visibility;
    parameter_.Descriptor.ShaderRegister = Register;
    parameter_.Descriptor.RegisterSpace = Space;
}

void RootParameter::InitAsBufferSRV( UINT Register, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL, UINT Space = 0 )
{
    parameter_.ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
    parameter_.ShaderVisibility = Visibility;
    parameter_.Descriptor.ShaderRegister = Register;
    parameter_.Descriptor.RegisterSpace = Space;
}

void RootParameter::InitAsBufferUAV( UINT Register, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL, UINT Space = 0 )
{
    parameter_.ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
    parameter_.ShaderVisibility = Visibility;
    parameter_.Descriptor.ShaderRegister = Register;
    parameter_.Descriptor.RegisterSpace = Space;
}

void RootParameter::InitAsDescriptorRange( D3D12_DESCRIPTOR_RANGE_TYPE Type, UINT Register, UINT Count, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL, UINT Space = 0 )
{
    InitAsDescriptorTable(1, Visibility);
    SetTableRange(0, Type, Register, Count, Space);
}

void RootParameter::InitAsDescriptorTable( UINT RangeCount, D3D12_SHADER_VISIBILITY Visibility = D3D12_SHADER_VISIBILITY_ALL )
{
    parameter_.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    parameter_.ShaderVisibility = Visibility;
    parameter_.DescriptorTable.NumDescriptorRanges = RangeCount;
    parameter_.DescriptorTable.pDescriptorRanges = new D3D12_DESCRIPTOR_RANGE[RangeCount];
}

void RootParameter::SetTableRange( UINT RangeIndex, D3D12_DESCRIPTOR_RANGE_TYPE Type, UINT Register, UINT Count, UINT Space = 0 )
{
    D3D12_DESCRIPTOR_RANGE* range = const_cast<D3D12_DESCRIPTOR_RANGE*>(parameter_.DescriptorTable.pDescriptorRanges + RangeIndex);
    range->RangeType = Type;
    range->NumDescriptors = Count;
    range->BaseShaderRegister = Register;
    range->RegisterSpace = Space;
    range->OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
}
*/

RootSignature::RootSignature(u32 numRootParams) : numParameters_(numRootParams) {
    Reset(numRootParams);
}

void RootSignature::Reset(u32 numRootParams) {
    if (numRootParams > 0) {
        parameters_.reset(new CD3DX12_ROOT_PARAMETER[numRootParams]);
    }
    else {
        parameters_ = nullptr;
    }
    numParameters_ = numRootParams;
}

void RootSignature::Build() {

}

const CD3DX12_ROOT_PARAMETER &RootSignature::operator[](size_t EntryIndex) const {
    assert(EntryIndex < numParameters_);
    return parameters_.get()[EntryIndex];
}

CD3DX12_ROOT_PARAMETER &RootSignature::operator[](size_t EntryIndex) {
    assert(EntryIndex < numParameters_);
    return parameters_.get()[EntryIndex];
}


void RootSignature::Build(ID3D12Device *device) {

}

}

