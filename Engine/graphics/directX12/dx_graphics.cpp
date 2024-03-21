/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details) 
 ************************************************************************/
/**
 * @file dx_graphics.cpp
 * @version 1.0
 * @date 25/02/2024
 * @brief Short description
 *
 * Longer description
 */

#include "dx_graphics.hpp"
#include "render/mesh.hpp"

namespace reveal3d::graphics::dx {

using namespace render;

Graphics::Graphics(const window::Resolution &res) :
    resolution_(res), presentInfo_(0), allowTearing_(0)
{

}

void Graphics::LoadPipeline() {
    // Factory -> LookForAdapter -> CreateDevice -> CommandQueue -> SwapChain
    InitDXGIAdapter();
    cmdManager_.Init(device_.Get());
    heaps_.rtv.Initialize(device_.Get(), frameBufferCount, false);
    heaps_.cbv.Initialize(device_.Get(), frameBufferCount, true);
    heaps_.dsv.Initialize(device_.Get(), 1U, false);
    CreateSwapChain();
    InitFrameResources();
    SetViewport();
}

// TODO: search for first avaible hardware adapter and look for best performance adapter (GPU)
// TODO: check for more features
void Graphics::InitDXGIAdapter() {
    u32 factoryFlags = 0;

#ifdef _DEBUG
    utl::LogAdapters();
    utl::EnableCpuLayer(factoryFlags);
    utl::EnableGpuLayer();
#endif
    CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&factory_)) >> utl::DxCheck;
    D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device_)) >> utl::DxCheck;

    if (SUCCEEDED(factory_->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing_, sizeof(u32))) &&
        allowTearing_) {
        presentInfo_ = DXGI_PRESENT_ALLOW_TEARING;
    }

#ifdef _DEBUG
    utl::QueueInfo(device_.Get(), TRUE);
#endif
}

void Graphics::CreateSwapChain() {
    ComPtr<IDXGISwapChain1> swapChain1;
    const DXGI_SWAP_CHAIN_DESC1 swapChainDesc{
            .Width = resolution_.width,
            .Height = resolution_.height,
            .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
            .Stereo = FALSE,
            .SampleDesc = {1, 0},
            .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
            .BufferCount = frameBufferCount,
            .Scaling = DXGI_SCALING_STRETCH,
            .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
            .AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
            .Flags = allowTearing_ ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0U
    };
    factory_->CreateSwapChainForHwnd(cmdManager_.GetQueue(), window_, &swapChainDesc, nullptr, nullptr, &swapChain1) >> utl::DxCheck;
    factory_->MakeWindowAssociation(window_, DXGI_MWA_NO_ALT_ENTER) >> utl::DxCheck; // Disable Alt + Enter for full screen window
    swapChain1.As(&swapChain_) >> utl::DxCheck;
}

void Graphics::InitFrameResources() {
    for(u32 i = 0; i < frameBufferCount; ++i) {
        D3D12_RENDER_TARGET_VIEW_DESC desc = {
                .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
                .ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D
        };
        frameResources_[i].handle = heaps_.rtv.alloc();
        swapChain_->GetBuffer(i, IID_PPV_ARGS(&frameResources_[i].backBuffer)) >> utl::DxCheck;
        device_->CreateRenderTargetView(frameResources_[i].backBuffer.Get() ,&desc, frameResources_[i].handle.cpu);
        frameResources_[i].constantBuffer_.Init(device_.Get(), 1U);
    }
}

void Graphics::SetViewport() {
    DXGI_SWAP_CHAIN_DESC desc {};

    swapChain_->GetDesc(&desc)  >> utl::DxCheck;
    viewport_.TopLeftX = 0.0f;
    viewport_.TopLeftY = 0.0f;
    viewport_.Height = (f32) desc.BufferDesc.Height;
    viewport_.Width = (f32) desc.BufferDesc.Width;
    viewport_.MaxDepth = 1.0f;
    viewport_.MinDepth = 0.0f;
    scissorRect_ = { 0, 0, (i32) desc.BufferDesc.Width, (i32) desc.BufferDesc.Height };
}

void Graphics::LoadAssets() {
    BuildRootSignature();
    BuildPSO();
    cmdManager_.Reset(nullptr);
    Vertex vertices[] = {
            { math::vec3(-1.0f, -1.0f, -1.0f), math::vec4(1.0f, 1.0f, 1.0f, 0.0f) },
            { math::vec3(-1.0f, +1.0f, -1.0f), math::vec4(0.0f, 0.0f, 0.0f, 0.0f) },
            { math::vec3(+1.0f, +1.0f, -1.0f), math::vec4(1.0f, 0.0f, 0.0f, 0.0f) },
            { math::vec3(+1.0f, -1.0f, -1.0f), math::vec4(0.0f, 1.0f, 0.0f, 0.0f) },
            { math::vec3(-1.0f, -1.0f, +1.0f), math::vec4(0.0f, 1.0f, 0.0f, 0.0f) },
            { math::vec3(-1.0f, +1.0f, +1.0f), math::vec4(0.0f, 1.0f, 0.0f, 0.0f) },
            { math::vec3(+1.0f, +1.0f, +1.0f), math::vec4(0.0f, 1.0f, 0.0f, 0.0f) },
            { math::vec3(+1.0f, -1.0f, +1.0f), math::vec4(0.0f, 1.0f, 0.0f, 0.0f) }
    };
    u16 indices[] = {
            /* front face  */ 0, 1, 2, 0, 2, 3,
            /* back face   */ 4, 6, 5, 4, 7, 6,
            /* left face   */ 4, 5, 1, 4, 1, 0,
            /* right face  */ 3, 2, 6, 3, 6, 7,
            /* top face    */ 1, 5, 6, 1, 6, 2,
            /* bottom face */ 4, 0, 3, 4, 3, 7
    };
    BufferInitInfo vertexBufferInfo = {
            .device = device_.Get(),
            .cmdList = cmdManager_.List(),
            .data = &vertices,
            .count = sizeof(vertices) / sizeof(Vertex),
    };
    BufferInitInfo indexBufferInfo = {
            .device = device_.Get(),
            .cmdList = cmdManager_.List(),
            .data = &indices,
            .count = sizeof(indices) / sizeof(u16),
            .format = DXGI_FORMAT_R16_UINT
    };
    vertexBuffer_.Init(vertexBufferInfo);
    indexBuffer_.Init(indexBufferInfo);

    for (auto& frameResource : frameResources_) { // TODO: Create constant buffer view for each scene object
        const D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDes = {
            .BufferLocation = frameResource.constantBuffer_.GpuAddress(),
            .SizeInBytes = frameResource.constantBuffer_.Size()
        };
        device_->CreateConstantBufferView(&cbvDes, heaps_.cbv.CpuStart());
    }

    cmdManager_.List()->Close();
    cmdManager_.Execute();
    cmdManager_.WaitForGPU();

}

void Graphics::BuildRootSignature() {
    CD3DX12_DESCRIPTOR_RANGE1 ranges[1];
    CD3DX12_ROOT_PARAMETER1 rootParameters[1];
    D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {
            .HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1
    };

    if (FAILED(device_->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData)))) {
        featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
    }

    ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
    rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_VERTEX);

    // Allow input layout and deny uneccessary access to certain pipeline stages.
    D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
            D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
            D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
            D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
            D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
            D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

    CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
    rootSignatureDesc.Init_1_1(_countof(rootParameters), rootParameters, 0, nullptr, rootSignatureFlags);

    ComPtr<ID3DBlob> signature;
    ComPtr<ID3DBlob> error;
    D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, featureData.HighestVersion, &signature, &error) >> utl::DxCheck;
    device_->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature_)) >> utl::DxCheck;

}

void Graphics::BuildPSO() {
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

    hr = D3DCompileFromFile(L"D:/Universidad/tfg/engine/Reveal3d/Engine/graphics/cshaders/FlatShader.hlsl", nullptr, nullptr, "VS", "vs_5_0", compileFlags, 0, &vertexShader, &errors);
    if (errors != nullptr) log(logDEBUG) << (char *) errors->GetBufferPointer();
    hr >> utl::DxCheck;
    hr = D3DCompileFromFile(L"D:/Universidad/tfg/engine/Reveal3d/Engine/graphics/cshaders/FlatShader.hlsl", nullptr, nullptr, "PS", "ps_5_0", compileFlags, 0, &pixelShader, &errors);
    if (errors != nullptr) log(logDEBUG) << (char *) errors->GetBufferPointer();
    hr >> utl::DxCheck;

    // Define the vertex input layout.
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    // Describe and create the graphics pipeline state object (PSO).
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
    psoDesc.pRootSignature = rootSignature_.Get();
    psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader.Get());
    psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader.Get());
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState.DepthEnable = FALSE;
    psoDesc.DepthStencilState.StencilEnable = FALSE;
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.SampleDesc.Count = 1;

    device_->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState_)) >> utl::DxCheck;
}

void Graphics::Update(render::Camera &camera) {
    f32 x = 0.5f * sinf(XM_PIDIV4) * cosf(1.5f * XM_PI);
    f32 z = 0.5f * sinf(XM_PIDIV4) * sinf(1.5f * XM_PI);
    f32 y = 0.5f * cosf(XM_PIDIV4);

    // Build the view matrix.
    XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
    XMVECTOR target = XMVectorZero();
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    XMMATRIX view = XMMatrixLookAtLH(pos, target, up);

    XMMATRIX world = XMMatrixIdentity();
    XMMATRIX proj = XMMatrixPerspectiveFovLH(0.25f * 3.141516, resolution_.aspectRatio, 1.0f, 1000.0f);
    XMMATRIX worldViewProj = XMMatrixTranspose(world * view * proj);

    // Update the constant buffer with the latest worldViewProj matrix.
    AlignedObjCosntant objConstants = {.constants = {worldViewProj}};
    frameResources_[Commands::FrameIndex()].constantBuffer_.CopyData(0, &objConstants, 1);
}

// TODO: Functions to simplify barrier creation
void Graphics::PrepareRender() {

    auto &currFrameRes = frameResources_[Commands::FrameIndex()];
    ID3D12GraphicsCommandList* commandList = cmdManager_.List();

    cmdManager_.Reset(pipelineState_.Get()); //Resets commands list and current frame allocator
    CleanDeferredResources(heaps_); // Clean deferreds resources

    commandList->RSSetViewports(1, &viewport_);
    commandList->RSSetScissorRects(1, &scissorRect_);

    auto targetBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
            currFrameRes.backBuffer.Get(), D3D12_RESOURCE_STATE_PRESENT,
            D3D12_RESOURCE_STATE_RENDER_TARGET );
    commandList->ResourceBarrier(1, &targetBarrier);

    const f32 clearColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    commandList->ClearRenderTargetView(currFrameRes.handle.cpu, clearColor, 0, nullptr);
    commandList->OMSetRenderTargets(1, &currFrameRes.handle.cpu, FALSE, nullptr);

    ID3D12DescriptorHeap* descHeaps[] = { heaps_.cbv.Get() };
    commandList->SetDescriptorHeaps(_countof(descHeaps), descHeaps);

    commandList->SetGraphicsRootSignature(rootSignature_.Get());
    commandList->IASetVertexBuffers(0, 1, vertexBuffer_.View());
    commandList->IASetIndexBuffer(indexBuffer_.View());
    commandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    commandList->SetGraphicsRootDescriptorTable(0, heaps_.cbv.GpuStart());

    commandList->DrawIndexedInstanced(36, 1, 0 ,0 ,0); // Hardcoded TODO

    auto presentBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
            currFrameRes.backBuffer.Get(),
            D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT );
    commandList->ResourceBarrier(1, &presentBarrier);
    commandList->Close() >> utl::DxCheck;
}

void Graphics::Draw() {
    cmdManager_.Execute();
    swapChain_->Present(0, presentInfo_) >> utl::DxCheck;
    cmdManager_.MoveToNextFrame();
}

void Graphics::Terminate() {
#ifdef _DEBUG
    utl::QueueInfo(device_.Get(), FALSE);
    utl::SetReporter(device_.Get());
#endif
    cmdManager_.Flush();
    heaps_.Release();
    vertexBuffer_.Release();
    indexBuffer_.Release();
    for (auto& frameResource : frameResources_) {
        frameResource.constantBuffer_.Release();
    }
    CleanDeferredResources(heaps_);
}


}
