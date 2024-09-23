//
// Created by acbsu on 12/08/2024.
//

#include "dx_gpass.hpp"

#include "core/scene.hpp"
#include "config/config.hpp"


namespace reveal3d::graphics::dx12 {

const f32 Gpass::clearColor_[] = { config::clearColor.x, config::clearColor.y, config::clearColor.z, config::clearColor.w };

Gpass::Gpass() {
    rootSignatures_[render::Shader::opaque].Reset(4);
    rootSignatures_[render::Shader::unlit].Reset(4);
    rootSignatures_[render::Shader::grid].Reset(4);
}

void Gpass::Init(ID3D12Device* device) {
    BuildRoots(device);
    BuildPsos(device);
}

void Gpass::SetRenderTargets(Commands &commandMng, FrameResource& frameResource) {
    ID3D12GraphicsCommandList* commandList = commandMng.List();

    commandList->ClearRenderTargetView(frameResource.backBufferHandle.cpu, clearColor_, 0, nullptr);
    commandList->ClearDepthStencilView(frameResource.depthBufferHandle.cpu, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
    commandList->OMSetRenderTargets(1, &frameResource.backBufferHandle.cpu, TRUE, &frameResource.depthBufferHandle.cpu);

}

void Gpass::Render(ID3D12GraphicsCommandList* commandList, FrameResource& frameResource) {
    currRootSignature_ = nullptr;
    currPipelineState_ = nullptr;

    commandList->ClearRenderTargetView(frameResource.backBufferHandle.cpu, clearColor_, 0, nullptr);
    commandList->ClearDepthStencilView(frameResource.depthBufferHandle.cpu, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
    commandList->OMSetRenderTargets(1, &frameResource.backBufferHandle.cpu, TRUE, &frameResource.depthBufferHandle.cpu);

    for (auto& renderElement : renderElements_) {

        auto geometry = core::scene.ComponentPool<core::Geometry>().At(renderElement.meshId);

        for (auto &submesh : geometry.SubMeshes()) {
            if (!submesh.visible) continue;

            if (currRootSignature_ != rootSignatures_.at(submesh.shader).Get()) {
                currRootSignature_ = rootSignatures_.at(submesh.shader).Get();
                commandList->SetGraphicsRootSignature(currRootSignature_);
            }

            if (currPipelineState_ != pipelineStates_.at(submesh.shader).Get()) {
                currPipelineState_ = pipelineStates_.at(submesh.shader).Get();
                commandList->SetPipelineState(currPipelineState_);
            }

            commandList->SetGraphicsRootConstantBufferView(0, frameResource.constantBuffer.GpuPos(id::index(renderElement.meshId)));
            commandList->SetGraphicsRootConstantBufferView(1, frameResource.matBuffer.GpuPos(id::index(renderElement.meshId)));
            commandList->SetGraphicsRootConstantBufferView(2, frameResource.passBuffer.GpuStart());

            commandList->IASetVertexBuffers(0, 1, renderElement.vertexBuffer.View());
            commandList->IASetIndexBuffer(renderElement.indexBuffer.View());
            commandList->IASetPrimitiveTopology(renderElement.topology);
            commandList->DrawIndexedInstanced(submesh.indexCount, 1, submesh.indexPos, submesh.vertexPos, 0);
        }
    }
    DrawWorldGrid(commandList, frameResource);
}

void Gpass::DrawWorldGrid(ID3D12GraphicsCommandList *commandList, FrameResource& frameResource) {
    commandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    commandList->SetGraphicsRootSignature(rootSignatures_[render::Shader::grid].Get());
    commandList->SetPipelineState(pipelineStates_[render::Shader::grid].Get());
    commandList->SetGraphicsRootConstantBufferView(2, frameResource.passBuffer.GpuStart());
    commandList->DrawInstanced(6, 1, 0, 0);
}

//NOTE change geometry to mesh when content module update
void Gpass::AddRenderElement(core::Geometry geometry, Commands& cmdMng, ID3D12Device* device) {
    BufferInitInfo vertexBufferInfo = {
            .device = device,
            .cmdList = cmdMng.List(),
            .data = geometry.Vertices().data(),
            .count = geometry.VertexCount()
    };

    BufferInitInfo indexBufferInfo = {
            .device = device,
            .cmdList = cmdMng.List(),
            .data = geometry.Indices().data(),
            .count = geometry.IndexCount(),
            .format = DXGI_FORMAT_R32_UINT
    };

    renderElements_.emplace_back(geometry.Id(), vertexBufferInfo, indexBufferInfo);
}

void Gpass::RemoveRenderElement(core::Geometry geometry) {

}

void Gpass::Terminate() {
    for (auto &elem : renderElements_) {
        elem.vertexBuffer.Release();
        elem.indexBuffer.Release();
    }
}

void Gpass::BuildPsos(ID3D12Device* device) {
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
    hr = D3DCompileFromFile(relative(L"../../Assets/shaders/hlsl/OpaqueShader.hlsl").c_str(), nullptr, nullptr, "VS", "vs_5_0", compileFlags, 0, &vertexShader, &errors);
    if (errors != nullptr) log(logDEBUG) << (char *) errors->GetBufferPointer();
    hr >> utl::DxCheck;
    hr = D3DCompileFromFile(relative(L"../../Assets/shaders/hlsl/OpaqueShader.hlsl").c_str(), nullptr, nullptr, "PS", "ps_5_0", compileFlags, 0, &pixelShader, &errors);
    if (errors != nullptr) log(logDEBUG) << (char *) errors->GetBufferPointer();
    hr >> utl::DxCheck;

    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    pipelineStates_[render::Shader::opaque].SetInputLayout(inputElementDescs, _countof(inputElementDescs));
    pipelineStates_[render::Shader::opaque].SetRootSignature(rootSignatures_[render::Shader::opaque]);
    pipelineStates_[render::Shader::opaque].SetShaders(vertexShader.Get(), pixelShader.Get());
    pipelineStates_[render::Shader::opaque].SetRasterizerCullMode(D3D12_CULL_MODE_NONE);
    pipelineStates_[render::Shader::opaque].SetBlendState(CD3DX12_BLEND_DESC(D3D12_DEFAULT));
    pipelineStates_[render::Shader::opaque].SetDepthStencil(CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT));
    pipelineStates_[render::Shader::opaque].SetSampleMask(UINT_MAX);
    pipelineStates_[render::Shader::opaque].SetPrimitive(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
    pipelineStates_[render::Shader::opaque].SetNumRenderTargets(1U);
    pipelineStates_[render::Shader::opaque].SetRtvFormats(0U, DXGI_FORMAT_R8G8B8A8_UNORM);
    pipelineStates_[render::Shader::opaque].SetSampleDescCount(1U); //TODO: Support x4
    pipelineStates_[render::Shader::opaque].SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);

    pipelineStates_[render::Shader::opaque].Finalize(device);

    //TODO Config file for assets path
    hr = D3DCompileFromFile(relative(L"../../Assets/shaders/hlsl/FlatShader.hlsl").c_str(), nullptr, nullptr, "VS", "vs_5_0", compileFlags, 0, &vertexShader, &errors);
    if (errors != nullptr) log(logDEBUG) << (char *) errors->GetBufferPointer();
    hr >> utl::DxCheck;
    hr = D3DCompileFromFile(relative(L"../../Assets/shaders/hlsl/FlatShader.hlsl").c_str(), nullptr, nullptr, "PS", "ps_5_0", compileFlags, 0, &pixelShader, &errors);
    if (errors != nullptr) log(logDEBUG) << (char *) errors->GetBufferPointer();
    hr >> utl::DxCheck;

    D3D12_INPUT_ELEMENT_DESC flatElementsDesc[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    pipelineStates_[render::Shader::unlit].SetInputLayout(flatElementsDesc, _countof(flatElementsDesc));
    pipelineStates_[render::Shader::unlit].SetRootSignature(rootSignatures_[render::Shader::opaque]);
    pipelineStates_[render::Shader::unlit].SetShaders(vertexShader.Get(), pixelShader.Get());
    pipelineStates_[render::Shader::unlit].SetRasterizerCullMode(D3D12_CULL_MODE_NONE);
    pipelineStates_[render::Shader::unlit].SetBlendState(CD3DX12_BLEND_DESC(D3D12_DEFAULT));
    pipelineStates_[render::Shader::unlit].SetDepthStencil(CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT));
    pipelineStates_[render::Shader::unlit].SetSampleMask(UINT_MAX);
    pipelineStates_[render::Shader::unlit].SetPrimitive(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
    pipelineStates_[render::Shader::unlit].SetNumRenderTargets(1U);
    pipelineStates_[render::Shader::unlit].SetRtvFormats(0U, DXGI_FORMAT_R8G8B8A8_UNORM);
    pipelineStates_[render::Shader::unlit].SetSampleDescCount(1U); //TODO: Support x4
    pipelineStates_[render::Shader::unlit].SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);

    pipelineStates_[render::Shader::unlit].Finalize(device);

    hr = D3DCompileFromFile(relative(L"../../Assets/shaders/hlsl/GridShader.hlsl").c_str(), nullptr, nullptr, "VS", "vs_5_0", compileFlags, 0, &vertexShader, &errors);
    if (errors != nullptr) log(logDEBUG) << (char *) errors->GetBufferPointer();
    hr >> utl::DxCheck;
    hr = D3DCompileFromFile(relative(L"../../Assets/shaders/hlsl/GridShader.hlsl").c_str(), nullptr, nullptr, "PS", "ps_5_0", compileFlags, 0, &pixelShader, &errors);
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

    pipelineStates_[render::Shader::grid].SetInputLayout(flatElementsDesc, _countof(flatElementsDesc));
    pipelineStates_[render::Shader::grid].SetRootSignature(rootSignatures_[render::Shader::grid]);
    pipelineStates_[render::Shader::grid].SetShaders(vertexShader.Get(), pixelShader.Get());
    pipelineStates_[render::Shader::grid].SetRasterizerCullMode(D3D12_CULL_MODE_NONE);
    pipelineStates_[render::Shader::grid].SetBlendState(blend_desc);
    pipelineStates_[render::Shader::grid].SetDepthStencil(CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT));
    pipelineStates_[render::Shader::grid].SetSampleMask(UINT_MAX);
    pipelineStates_[render::Shader::grid].SetPrimitive(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
    pipelineStates_[render::Shader::grid].SetNumRenderTargets(1U);
    pipelineStates_[render::Shader::grid].SetRtvFormats(0U, DXGI_FORMAT_R8G8B8A8_UNORM);
    pipelineStates_[render::Shader::grid].SetSampleDescCount(1U); //TODO: Support x4
    pipelineStates_[render::Shader::grid].SetDSVFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);

    pipelineStates_[render::Shader::grid].Finalize(device);

//    auto gridMesh = new render::SubMesh();
//    gridMesh->indexCount = 6;
//    meshes_[render::Shader::grid].push_back(gridMesh);
}

void Gpass::BuildRoots(ID3D12Device* device) {
    rootSignatures_[render::Shader::opaque][0].InitAsConstantBufferView(0);
    rootSignatures_[render::Shader::opaque][1].InitAsConstantBufferView(1);
    rootSignatures_[render::Shader::opaque][2].InitAsConstantBufferView(2);
    rootSignatures_[render::Shader::opaque][3].InitAsShaderResourceView(3);
    rootSignatures_[render::Shader::opaque].Finalize(device);

    rootSignatures_[render::Shader::unlit][0].InitAsConstantBufferView(0);
    rootSignatures_[render::Shader::unlit][1].InitAsConstantBufferView(1);
    rootSignatures_[render::Shader::unlit][2].InitAsConstantBufferView(2);
    rootSignatures_[render::Shader::unlit][3].InitAsShaderResourceView(3);
    rootSignatures_[render::Shader::unlit].Finalize(device);
    //    worldGridLayer_.rootSignature.Reset(1);
    rootSignatures_[render::Shader::grid][0].InitAsConstantBufferView(0);
    rootSignatures_[render::Shader::grid][1].InitAsConstantBufferView(1);
    rootSignatures_[render::Shader::grid][2].InitAsConstantBufferView(2);
    rootSignatures_[render::Shader::grid][3].InitAsShaderResourceView(3);
    rootSignatures_[render::Shader::grid].Finalize(device);


}


}
