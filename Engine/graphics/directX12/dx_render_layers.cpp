/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_render_layers.cpp
 * @version 1.0
 * @date 02/04/2024
 * @brief Short description
 *
 * Longer description
 */

#include "dx_render_layers.hpp"

namespace reveal3d::graphics::dx {

RenderLayers::RenderLayers() {
    layers_[render::Shader::opaque].rootSignature.Reset(3);
    layers_[render::Shader::flat].rootSignature.Reset(3);
    layers_[render::Shader::grid].rootSignature.Reset(3);
//    worldGridLayer_.rootSignature.Reset(1);
}

void RenderLayers::BuildRoots(ID3D12Device *device) {
    layers_[render::Shader::opaque].rootSignature[0].InitAsConstantBufferView(0);
    layers_[render::Shader::opaque].rootSignature[1].InitAsConstantBufferView(1);
    layers_[render::Shader::opaque].rootSignature[2].InitAsShaderResourceView(2);
    layers_[render::Shader::opaque].rootSignature.Finalize(device);

    layers_[render::Shader::flat].rootSignature[0].InitAsConstantBufferView(0);
    layers_[render::Shader::flat].rootSignature[1].InitAsConstantBufferView(1);
    layers_[render::Shader::flat].rootSignature[2].InitAsShaderResourceView(2);
    layers_[render::Shader::flat].rootSignature.Finalize(device);
//    worldGridLayer_.rootSignature.Reset(1);
    layers_[render::Shader::grid].rootSignature[0].InitAsConstantBufferView(0);
    layers_[render::Shader::grid].rootSignature[1].InitAsConstantBufferView(1);
    layers_[render::Shader::grid].rootSignature[2].InitAsShaderResourceView(2);
    layers_[render::Shader::grid].rootSignature.Finalize(device);

}

void RenderLayers::BuildPSOs(ID3D12Device *device) {
    ComPtr<ID3DBlob> vertexShader;
    ComPtr<ID3DBlob> pixelShader;
    ComPtr<ID3DBlob> errors;

#if defined(_DEBUG)
    // Enable better Shader debugging with the graphics debugging tools.
    UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    UINT compileFlags = 0;
#endif
    HRESULT hr = S_OK;

    //TODO Config file for assets path
    hr = D3DCompileFromFile(relative(L"Engine/graphics/cshaders/OpaqueShader.hlsl").c_str(), nullptr, nullptr, "VS", "vs_5_0", compileFlags, 0, &vertexShader, &errors);
    if (errors != nullptr) log(logDEBUG) << (char *) errors->GetBufferPointer();
    hr >> utl::DxCheck;
    hr = D3DCompileFromFile(relative(L"Engine/graphics/cshaders/OpaqueShader.hlsl").c_str(), nullptr, nullptr, "PS", "ps_5_0", compileFlags, 0, &pixelShader, &errors);
    if (errors != nullptr) log(logDEBUG) << (char *) errors->GetBufferPointer();
    hr >> utl::DxCheck;

    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    layers_[render::Shader::opaque].pso.SetInputLayout(inputElementDescs, _countof(inputElementDescs));
    layers_[render::Shader::opaque].pso.SetRootSignature(layers_[render::Shader::opaque].rootSignature);
    layers_[render::Shader::opaque].pso.SetShaders(vertexShader.Get(), pixelShader.Get());
    layers_[render::Shader::opaque].pso.SetRasterizerCullMode(D3D12_CULL_MODE_NONE);
    layers_[render::Shader::opaque].pso.SetBlendState(CD3DX12_BLEND_DESC(D3D12_DEFAULT));
    layers_[render::Shader::opaque].pso.SetDepthStencil(CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT));
    layers_[render::Shader::opaque].pso.SetSampleMask(UINT_MAX);
    layers_[render::Shader::opaque].pso.SetPrimitive(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
    layers_[render::Shader::opaque].pso.SetNumRenderTargets(1U);
    layers_[render::Shader::opaque].pso.SetRtvFormats(0U, DXGI_FORMAT_R8G8B8A8_UNORM);
    layers_[render::Shader::opaque].pso.SetSampleDescCount(1U); //TODO: Support x4
    layers_[render::Shader::opaque].pso.SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);

    layers_[render::Shader::opaque].pso.Finalize(device);

    //TODO Config file for assets path
    hr = D3DCompileFromFile(relative(L"Engine/graphics/cshaders/FlatShader.hlsl").c_str(), nullptr, nullptr, "VS", "vs_5_0", compileFlags, 0, &vertexShader, &errors);
    if (errors != nullptr) log(logDEBUG) << (char *) errors->GetBufferPointer();
    hr >> utl::DxCheck;
    hr = D3DCompileFromFile(relative(L"Engine/graphics/cshaders/FlatShader.hlsl").c_str(), nullptr, nullptr, "PS", "ps_5_0", compileFlags, 0, &pixelShader, &errors);
    if (errors != nullptr) log(logDEBUG) << (char *) errors->GetBufferPointer();
    hr >> utl::DxCheck;

    D3D12_INPUT_ELEMENT_DESC flatElementsDesc[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    layers_[render::Shader::flat].pso.SetInputLayout(flatElementsDesc, _countof(flatElementsDesc));
    layers_[render::Shader::flat].pso.SetRootSignature(layers_[render::Shader::opaque].rootSignature);
    layers_[render::Shader::flat].pso.SetShaders(vertexShader.Get(), pixelShader.Get());
    layers_[render::Shader::flat].pso.SetRasterizerCullMode(D3D12_CULL_MODE_NONE);
    layers_[render::Shader::flat].pso.SetBlendState(CD3DX12_BLEND_DESC(D3D12_DEFAULT));
    layers_[render::Shader::flat].pso.SetDepthStencil(CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT));
    layers_[render::Shader::flat].pso.SetSampleMask(UINT_MAX);
    layers_[render::Shader::flat].pso.SetPrimitive(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
    layers_[render::Shader::flat].pso.SetNumRenderTargets(1U);
    layers_[render::Shader::flat].pso.SetRtvFormats(0U, DXGI_FORMAT_R8G8B8A8_UNORM);
    layers_[render::Shader::flat].pso.SetSampleDescCount(1U); //TODO: Support x4
    layers_[render::Shader::flat].pso.SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);

    layers_[render::Shader::flat].pso.Finalize(device);

    hr = D3DCompileFromFile(relative(L"Engine/graphics/cshaders/GridShader.hlsl").c_str(), nullptr, nullptr, "VS", "vs_5_0", compileFlags, 0, &vertexShader, &errors);
    if (errors != nullptr) log(logDEBUG) << (char *) errors->GetBufferPointer();
    hr >> utl::DxCheck;
    hr = D3DCompileFromFile(relative(L"Engine/graphics/cshaders/GridShader.hlsl").c_str(), nullptr, nullptr, "PS", "ps_5_0", compileFlags, 0, &pixelShader, &errors);
    if (errors != nullptr) log(logDEBUG) << (char *) errors->GetBufferPointer();
    hr >> utl::DxCheck;

    D3D12_RENDER_TARGET_BLEND_DESC transparencyBlendDesc;
    transparencyBlendDesc.BlendEnable = true;
    transparencyBlendDesc.LogicOpEnable = false;
    transparencyBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
    transparencyBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    transparencyBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
    transparencyBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
    transparencyBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
    transparencyBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
    transparencyBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
    transparencyBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    D3D12_BLEND_DESC blend_desc = {
            .RenderTarget = transparencyBlendDesc
    };

    layers_[render::Shader::grid].pso.SetInputLayout(flatElementsDesc, _countof(flatElementsDesc));
    layers_[render::Shader::grid].pso.SetRootSignature(layers_[render::Shader::grid].rootSignature);
    layers_[render::Shader::grid].pso.SetShaders(vertexShader.Get(), pixelShader.Get());
    layers_[render::Shader::grid].pso.SetRasterizerCullMode(D3D12_CULL_MODE_NONE);
    layers_[render::Shader::grid].pso.SetBlendState(blend_desc);
    layers_[render::Shader::grid].pso.SetDepthStencil(CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT));
    layers_[render::Shader::grid].pso.SetSampleMask(UINT_MAX);
    layers_[render::Shader::grid].pso.SetPrimitive(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
    layers_[render::Shader::grid].pso.SetNumRenderTargets(1U);
    layers_[render::Shader::grid].pso.SetRtvFormats(0U, DXGI_FORMAT_R8G8B8A8_UNORM);
    layers_[render::Shader::grid].pso.SetSampleDescCount(1U); //TODO: Support x4
    layers_[render::Shader::grid].pso.SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);

    layers_[render::Shader::grid].pso.Finalize(device);

    render::SubMesh *gridMesh = new render::SubMesh();
//    auto gridMesh = render::SubMesh();
    gridMesh->indexCount = 6;
    meshes_[render::Shader::grid].push_back(gridMesh);
}

void RenderLayers::AddMesh(render::SubMesh &mesh) {
    meshes_[mesh.shader].push_back(&mesh);
}

void RenderLayers::DrawLayer(ID3D12GraphicsCommandList *cmdList, FrameResource &frame, std::vector<RenderElement>& elements, u32 layer) {
    for (auto *mesh : meshes_[layer]) {
        if (!mesh->visible) continue;
        cmdList->SetGraphicsRootConstantBufferView(0, frame.constantBuffer.GpuPos(mesh->constantIndex));
        cmdList->IASetVertexBuffers(0, 1, elements.at(mesh->renderInfo).vertexBuffer.View());
        cmdList->IASetIndexBuffer(elements[mesh->renderInfo].indexBuffer.View());
        cmdList->IASetPrimitiveTopology(elements[mesh->renderInfo].topology);
        cmdList->DrawIndexedInstanced(mesh->indexCount, 1, mesh->indexPos, mesh->vertexPos, 0);
    }
}

void RenderLayers::DrawEffectLayer(ID3D12GraphicsCommandList *cmdList, u32 layer) {
    for (auto *mesh : meshes_[layer]) {
        cmdList->DrawIndexedInstanced(mesh->indexCount, 1, mesh->indexPos, mesh->vertexPos, 0);
    }

}
RenderLayers::~RenderLayers() {
    delete meshes_[render::Shader::grid].at(0);
}


void Layer::Set(ID3D12GraphicsCommandList *cmdList) {
    cmdList->SetPipelineState(pso.Get());
    cmdList->SetGraphicsRootSignature(rootSignature.Get());
}


}