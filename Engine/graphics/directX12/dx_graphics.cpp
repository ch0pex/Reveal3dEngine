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

Graphics::Graphics(window::Resolution *res) :
    resolution_(res), presentInfo_(0),
    swapChainFlags_(DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH)
{

}

void Graphics::LoadPipeline() {
    // Factory -> LookForAdapter -> CreateDevice -> CommandQueue -> SwapChain
    InitDXGIAdapter();
    cmdManager_.Init(device_.Get());
    heaps_.rtv.Initialize(device_.Get(), frameBufferCount, false);
    heaps_.cbv.Initialize(device_.Get(), 4092, true);
    heaps_.dsv.Initialize(device_.Get(), 1U, false);
    renderElements_.reserve(512U);
    CreateSwapChain();
    InitFrameResources();
    InitConstantBuffers();
    SetViewport();
}

// TODO: search for first avaible hardware adapter and look for best performance adapter (GPU)
// TODO: check for more features
void Graphics::InitDXGIAdapter() {
    u32 factoryFlags = 0;
    BOOL allowTearing = FALSE;

#ifdef _DEBUG
    utl::LogAdapters();
    utl::EnableCpuLayer(factoryFlags);
    utl::EnableGpuLayer();
#endif
    CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&factory_)) >> utl::DxCheck;
    D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device_)) >> utl::DxCheck;

    if (SUCCEEDED(factory_->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(u32))) &&
        allowTearing) {
        presentInfo_ = DXGI_PRESENT_ALLOW_TEARING;
        swapChainFlags_ |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
    }

#ifdef _DEBUG
    utl::QueueInfo(device_.Get(), TRUE);
#endif
}

void Graphics::CreateSwapChain() {
    ComPtr<IDXGISwapChain1> swapChain1;

    const DXGI_SWAP_CHAIN_DESC1 swapChainDesc{
            .Width = resolution_->width,
            .Height = resolution_->height,
            .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
            .Stereo = FALSE,
            .SampleDesc = {1, 0},
            .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
            .BufferCount = frameBufferCount,
            .Scaling = DXGI_SCALING_STRETCH,
            .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
            .AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
            .Flags = swapChainFlags_
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
        frameResources_[i].backBufferHandle = heaps_.rtv.alloc();
        swapChain_->GetBuffer(i, IID_PPV_ARGS(&frameResources_[i].backBuffer)) >> utl::DxCheck;
        device_->CreateRenderTargetView(frameResources_[i].backBuffer.Get() ,&desc, frameResources_[i].backBufferHandle.cpu);
    }
}

void Graphics::InitConstantBuffers() {
    for(auto& frameResource : frameResources_) {
        frameResource.constantBuffer.Init(device_.Get(), 256U); //TODO: hardcoded capacity 256 maximum?
        frameResource.passBuffer.Init(device_.Get(),  1U); //TODO: hardcoded capacity
        frameResource.passHandle = frameResource.passBuffer.CreateView(device_.Get(), heaps_.cbv);
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

void Graphics::LoadAssets(core::Scene &scene) {
    BuildRootSignature();
    BuildPSO();
    cmdManager_.Reset(nullptr);

    const Vertex vertices[] = {
            { {-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f, 0.0f} }, // 0
            { {-1.0f,  1.0f, -1.0f}, {0.0f, 1.0f, 0.0f, 0.0f} }, // 1
            { {1.0f,  1.0f, -1.0f}, {1.0f, 1.0f, 0.0f, 0.0f} }, // 2
            { {1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f, 0.0f} }, // 3
            { {-1.0f, -1.0f,  1.0f}, {0.0f, 0.0f, 1.0f, 0.0f} }, // 4
            { {-1.0f,  1.0f,  1.0f}, {0.0f, 1.0f, 1.0f, 0.0f} }, // 5
            { {1.0f,  1.0f,  1.0f}, {1.0f, 1.0f, 1.0f, 0.0f} }, // 6
            { {1.0f, -1.0f,  1.0f}, {1.0f, 0.0f, 1.0f, 0.0f} }  // 7
    };

    const u16 indices[] = {
            0, 1, 2, 0, 2, 3,
            4, 6, 5, 4, 7, 6,
            4, 5, 1, 4, 1, 0,
            3, 2, 6, 3, 6, 7,
            1, 5, 6, 1, 6, 2,
            4, 0, 3, 4, 3, 7
    };

    BufferInitInfo vertexBufferInfo = {
            .device = device_.Get(),
            .cmdList = cmdManager_.List(),
            .data = &vertices,
            .count = _countof(vertices),
    };

    BufferInitInfo indexBufferInfo = {
            .device = device_.Get(),
            .cmdList = cmdManager_.List(),
            .data = &indices,
            .count = _countof(indices),
            .format = DXGI_FORMAT_R16_UINT
    };

    renderElements_.emplace_back(vertexBufferInfo, indexBufferInfo);
    AlignedConstant<ObjConstant, 1> objConstant;
    core::Transform transform;
    transform.SetRotation({45});
    for (u32 j = 0; j < frameBufferCount; ++j) {
        renderElements_[0].handles[j] = frameResources_[j].constantBuffer.CreateView(device_.Get(), heaps_.cbv);
        objConstant.data.world = math::Transpose(transform.World());
        frameResources_[j].constantBuffer.CopyData(0, &objConstant);
    }


    std::vector<Vertex> vertices2 = {
            { { -1.0f, -1.0f, 0.0f },{ 0.0f, 0.0f, 1.0, 0.0f} },
            { { -1.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, 1.0, 0.0f} },
            { { 1.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, 1.0, 0.0f} },
            { { 1.0f, -1.0f, 0.0f },{ 0.0f, 0.0f, 1.0, 0.0f} }
    };


    std::vector<u16> indices2 = {
            0, 1, 2,
            0, 2, 3
    };

    BufferInitInfo conevertinfo = {
            .device = device_.Get(),
            .cmdList = cmdManager_.List(),
            .data = &vertices2[0],
            .count = vertices2.size()
    };

    BufferInitInfo coneindinfo = {
            .device = device_.Get(),
            .cmdList = cmdManager_.List(),
            .data = &indices2[0],
            .count = indices2.size(),
            .format = DXGI_FORMAT_R16_UINT
    };

    renderElements_.emplace_back(conevertinfo, coneindinfo);
    AlignedConstant<ObjConstant, 1> objConstant2;
    core::Transform transform2;
    for (u32 j = 0; j < frameBufferCount; ++j) {
        renderElements_[1].handles[j] = frameResources_[j].constantBuffer.CreateView(device_.Get(), heaps_.cbv);
        objConstant2.data.world = math::Transpose(transform2.World());
        frameResources_[j].constantBuffer.CopyData(1, &objConstant2);
    }

    /*
    vertexBuffer_.Init(vertexBufferInfo);
    indexBuffer_.Init(indexBufferInfo);
    std::vector<core::Transform> &transforms = scene.Transforms();
    std::vector<core::Geometry> &geometries = scene.Geometries();

    for(u32 i = 0; i < scene.NumEntities(); ++i) {
        BufferInitInfo vertexBufferInfo = {
                .device = device_.Get(),
                .cmdList = cmdManager_.List(),
                .data = geometries[i].GetVertices(),
                .count = geometries[i].VertexCount()
        };

        BufferInitInfo indexBufferInfo = {
                .device = device_.Get(),
                .cmdList = cmdManager_.List(),
                .data = geometries[i].GetIndices(),
                .count = geometries[i].IndexCount(),
                .format = DXGI_FORMAT_R16_UINT
        };

        renderElements_.emplace_back(vertexBufferInfo, indexBufferInfo);

        AlignedConstant<ObjConstant, 1> objConstant;
        for (u32 j = 0; j < frameBufferCount; ++j) {
            renderElements_[i].handles[j] = frameResources_[j].constantBuffer.CreateView(device_.Get(), heaps_.cbv);
            objConstant.data.world = math::Transpose(transforms[i].World());
            frameResources_[j].constantBuffer.CopyData(0, &objConstant, 1);
        }
    }

    */
    cmdManager_.List()->Close();
    cmdManager_.Execute();
    cmdManager_.WaitForGPU();

}

void Graphics::BuildRootSignature() {
    CD3DX12_DESCRIPTOR_RANGE cbvTable0;
    cbvTable0.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

    CD3DX12_DESCRIPTOR_RANGE cbvTable1;
    cbvTable1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);

    // Root parameter can be a table, root descriptor or root constants.
    CD3DX12_ROOT_PARAMETER slotRootParameter[2];

    // Create root CBVs.
    slotRootParameter[0].InitAsDescriptorTable(1, &cbvTable0);
    slotRootParameter[1].InitAsDescriptorTable(1, &cbvTable1);

    // A root signature is an array of root parameters.
    CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(2, slotRootParameter, 0, nullptr,
                                            D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    // create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
    ComPtr<ID3DBlob> serializedRootSig = nullptr;
    ComPtr<ID3DBlob> errorBlob = nullptr;
    HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
                                             serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

    if(errorBlob != nullptr)
    {
        OutputDebugStringA((char*)errorBlob->GetBufferPointer());
    }
    hr >> utl::DxCheck;

    device_->CreateRootSignature( 0, serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(),
            IID_PPV_ARGS(rootSignature_.GetAddressOf())) >> utl::DxCheck;

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

    //TODO Config file for assets path
    hr = D3DCompileFromFile(L"D:/Universidad/tfg/Reveal3d/Engine/graphics/cshaders/FlatShader.hlsl", nullptr, nullptr, "VS", "vs_5_0", compileFlags, 0, &vertexShader, &errors);
    if (errors != nullptr) log(logDEBUG) << (char *) errors->GetBufferPointer();
    hr >> utl::DxCheck;
    hr = D3DCompileFromFile(L"D:/Universidad/tfg/Reveal3d/Engine/graphics/cshaders/FlatShader.hlsl", nullptr, nullptr, "PS", "ps_5_0", compileFlags, 0, &pixelShader, &errors);
    if (errors != nullptr) log(logDEBUG) << (char *) errors->GetBufferPointer();
    hr >> utl::DxCheck;

    // Define the vertex input layout.
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
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

void Graphics::Update(render::Camera &camera, const Timer& timer) {
    auto &currFrameRes = frameResources_[Commands::FrameIndex()];
    AlignedConstant<PassConstant, 2> passConstant;

    passConstant.data.viewProj = math::Transpose(camera.GetViewProjectionMatrix());
    // Update the constant buffer with the latest worldViewProj matrix.
    currFrameRes.passBuffer.CopyData(0, &passConstant);
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
    commandList->ClearRenderTargetView(currFrameRes.backBufferHandle.cpu, clearColor, 0, nullptr);
    commandList->OMSetRenderTargets(1, &currFrameRes.backBufferHandle.cpu, FALSE, nullptr);

    ID3D12DescriptorHeap* descHeaps[] = { heaps_.cbv.Get() };
    commandList->SetDescriptorHeaps(_countof(descHeaps), descHeaps);

    commandList->SetGraphicsRootSignature(rootSignature_.Get());

    commandList->SetGraphicsRootDescriptorTable(1, currFrameRes.passHandle.gpu);

    for (auto &element : renderElements_) {
        commandList->SetGraphicsRootDescriptorTable(0, element.handles[Commands::FrameIndex()].gpu);
        commandList->IASetVertexBuffers(0, 1, element.vertexBuffer.View());
        commandList->IASetIndexBuffer(element.indexBuffer.View());
        commandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        commandList->DrawIndexedInstanced(element.indexBuffer.Count(), 1, 0, 0 ,0); // Hardcoded TODO
    }


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

void Graphics::Resize(const window::Resolution &res) {
    if (res.aspectRatio == resolution_->aspectRatio) {
        return;
    }

    *resolution_ = res;
    cmdManager_.WaitForGPU();

    cmdManager_.Reset(nullptr);

    for(auto &frameResource : frameResources_){
        frameResource.backBuffer.Reset();
    }

    //depthStencilbuffer.reset()
    //BuildDepthBuffer

    swapChain_->ResizeBuffers(frameBufferCount, resolution_->width, resolution_->height, DXGI_FORMAT_R8G8B8A8_UNORM,
            swapChainFlags_) >> utl::DxCheck;

    cmdManager_.ResetFences();

    for (u32 i = 0; i < frameBufferCount; ++i) {
        D3D12_RENDER_TARGET_VIEW_DESC desc = {
                .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
                .ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D
        };
        swapChain_->GetBuffer(i, IID_PPV_ARGS(&frameResources_[i].backBuffer));
        device_->CreateRenderTargetView(frameResources_[i].backBuffer.Get() ,&desc, frameResources_[i].backBufferHandle.cpu);
    }

    cmdManager_.List()->Close();
    cmdManager_.Execute();
    cmdManager_.WaitForGPU();

    SetViewport();
}

void Graphics::Terminate() {
#ifdef _DEBUG
    utl::QueueInfo(device_.Get(), FALSE);
    utl::SetReporter(device_.Get());
#endif
    cmdManager_.Flush();
    heaps_.Release();

    for (auto &elem : renderElements_) {
        elem.vertexBuffer.Release();
        elem.indexBuffer.Release();
    }

    for (auto& frameResource : frameResources_) {
        frameResource.constantBuffer.Release();
        frameResource.passBuffer.Release();
    }
    CleanDeferredResources(heaps_);
}
}
