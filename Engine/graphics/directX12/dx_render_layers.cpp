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
    layers_[render::shader::opaque].rootSignature.Reset(2);
    layers_[render::shader::flat].rootSignature.Reset(2);
//    worldGridLayer_.rootSignature.Reset(1);
}

void RenderLayers::BuildRoots(ID3D12Device *device) {
    layers_[render::shader::opaque].rootSignature[0].InitAsConstantBufferView(0);
    layers_[render::shader::opaque].rootSignature[1].InitAsConstantBufferView(1);
    layers_[render::shader::opaque].rootSignature.Finalize(device);

    layers_[render::shader::flat].rootSignature[0].InitAsConstantBufferView(0);
    layers_[render::shader::flat].rootSignature[1].InitAsConstantBufferView(1);
    layers_[render::shader::flat].rootSignature.Finalize(device);
//    worldGridLayer_.rootSignature.Reset(1);

}

void RenderLayers::BuildPSOs(ID3D12Device *device) {
    ComPtr<ID3DBlob> vertexShader;
    ComPtr<ID3DBlob> pixelShader;
    ComPtr<ID3DBlob> errors;

#if defined(_DEBUG)
    // Enable better shader debugging with the graphics debugging tools.
    UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    UINT compileFlags = 0;
#endif
    HRESULT hr = S_OK;

    //TODO Config file for assets path
    hr = D3DCompileFromFile(L"D:/Universidad/tfg/engine/Reveal3d/Engine/graphics/cshaders/ColorDirLight.hlsl", nullptr, nullptr, "VS", "vs_5_0", compileFlags, 0, &vertexShader, &errors);
    if (errors != nullptr) log(logDEBUG) << (char *) errors->GetBufferPointer();
    hr >> utl::DxCheck;
    hr = D3DCompileFromFile(L"D:/Universidad/tfg/engine/Reveal3d/Engine/graphics/cshaders/ColorDirLight.hlsl", nullptr, nullptr, "PS", "ps_5_0", compileFlags, 0, &pixelShader, &errors);
    if (errors != nullptr) log(logDEBUG) << (char *) errors->GetBufferPointer();
    hr >> utl::DxCheck;

    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    layers_[render::shader::opaque].pso.SetInputLayout(inputElementDescs, _countof(inputElementDescs));
    layers_[render::shader::opaque].pso.SetRootSignature(layers_[render::shader::opaque].rootSignature);
    layers_[render::shader::opaque].pso.SetShaders(vertexShader.Get(), pixelShader.Get());
    layers_[render::shader::opaque].pso.SetRasterizerCullMode(D3D12_CULL_MODE_NONE);
    layers_[render::shader::opaque].pso.SetBlendState(CD3DX12_BLEND_DESC(D3D12_DEFAULT));
    layers_[render::shader::opaque].pso.SetDepthStencil(CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT));
    layers_[render::shader::opaque].pso.SetSampleMask(UINT_MAX);
    layers_[render::shader::opaque].pso.SetPrimitive(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
    layers_[render::shader::opaque].pso.SetNumRenderTargets(1U);
    layers_[render::shader::opaque].pso.SetRtvFormats(0U, DXGI_FORMAT_R8G8B8A8_UNORM);
    layers_[render::shader::opaque].pso.SetSampleDescCount(1U); //TODO: Support x4
    layers_[render::shader::opaque].pso.SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);

    layers_[render::shader::opaque].pso.Finalize(device);

    //TODO Config file for assets path
    hr = D3DCompileFromFile(L"D:/Universidad/tfg/engine/Reveal3d/Engine/graphics/cshaders/FlatShader.hlsl", nullptr, nullptr, "VS", "vs_5_0", compileFlags, 0, &vertexShader, &errors);
    if (errors != nullptr) log(logDEBUG) << (char *) errors->GetBufferPointer();
    hr >> utl::DxCheck;
    hr = D3DCompileFromFile(L"D:/Universidad/tfg/engine/Reveal3d/Engine/graphics/cshaders/FlatShader.hlsl", nullptr, nullptr, "PS", "ps_5_0", compileFlags, 0, &pixelShader, &errors);
    if (errors != nullptr) log(logDEBUG) << (char *) errors->GetBufferPointer();
    hr >> utl::DxCheck;

    D3D12_INPUT_ELEMENT_DESC flatElementsDesc[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    layers_[render::shader::flat].pso.SetInputLayout(flatElementsDesc, _countof(flatElementsDesc));
    layers_[render::shader::flat].pso.SetRootSignature(layers_[render::shader::opaque].rootSignature);
    layers_[render::shader::flat].pso.SetShaders(vertexShader.Get(), pixelShader.Get());
    layers_[render::shader::flat].pso.SetRasterizerCullMode(D3D12_CULL_MODE_NONE);
    layers_[render::shader::flat].pso.SetBlendState(CD3DX12_BLEND_DESC(D3D12_DEFAULT));
    layers_[render::shader::flat].pso.SetDepthStencil(CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT));
    layers_[render::shader::flat].pso.SetSampleMask(UINT_MAX);
    layers_[render::shader::flat].pso.SetPrimitive(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
    layers_[render::shader::flat].pso.SetNumRenderTargets(1U);
    layers_[render::shader::flat].pso.SetRtvFormats(0U, DXGI_FORMAT_R8G8B8A8_UNORM);
    layers_[render::shader::flat].pso.SetSampleDescCount(1U); //TODO: Support x4
    layers_[render::shader::flat].pso.SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);

    layers_[render::shader::flat].pso.Finalize(device);
}

void RenderLayers::AddMesh(render::Mesh &mesh) {
    meshes_[mesh.shader].push_back(&mesh);
}

void RenderLayers::DrawLayer(ID3D12GraphicsCommandList *cmdList, FrameResource &frame, u32 layer) {
    for (auto &mesh : meshes_[layer]) {
        if (!mesh->visible) continue;
        cmdList->SetGraphicsRootConstantBufferView(0, frame.constantBuffer.GpuPos(mesh->renderInfo->constantIndex));
        cmdList->IASetVertexBuffers(0, 1, mesh->renderInfo->vertexBuffer.View());
        cmdList->IASetIndexBuffer(mesh->renderInfo->indexBuffer.View());
        cmdList->IASetPrimitiveTopology(mesh->renderInfo->topology);
        cmdList->DrawIndexedInstanced(mesh->indexCount, 1, mesh->indexPos, mesh->vertexPos ,0);
    }

}
void Layer::Set(ID3D12GraphicsCommandList *cmdList) {
    cmdList->SetPipelineState(pso.Get());
    cmdList->SetGraphicsRootSignature(rootSignature.Get());
}


}
