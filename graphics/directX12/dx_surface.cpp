//
// Created by acbsu on 13/08/2024.
//

#include "dx_surface.hpp"

namespace reveal3d::graphics::dx12 {

Surface::Surface(window::Resolution &resolution, u32 swapChainFlags, u32 present_info)
        : resolution_(&resolution), swapChainFlags_(swapChainFlags), presentInfo_(present_info)
{

}

void Surface::Init(Commands& cmdManager, IDXGIFactory5* factory) {
    ComPtr<IDXGISwapChain1> swapChain1;

    const DXGI_SWAP_CHAIN_DESC1 swapChainDesc{
            .Width = resolution_->width,
            .Height = resolution_->height,
            .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
            .Stereo = FALSE,
            .SampleDesc = {1, 0},
            .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
            .BufferCount = dx12::frameBufferCount,
            .Scaling = DXGI_SCALING_STRETCH,
            .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
            .AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
            .Flags = swapChainFlags_
    };
    factory->CreateSwapChainForHwnd(cmdManager.GetQueue(), window_.hwnd, &swapChainDesc, nullptr, nullptr, &swapChain1) >> utl::DxCheck;
    factory->MakeWindowAssociation(window_.hwnd, DXGI_MWA_NO_ALT_ENTER) >> utl::DxCheck; // Disable Alt + Enter for full screen window
    swapChain1.As(&swapChain_) >> utl::DxCheck;
}

void Surface::UpdateViewport() {
    DXGI_SWAP_CHAIN_DESC desc {};

    swapChain_->GetDesc(&desc)  >> utl::DxCheck;
    viewport_.TopLeftX = 0.0F;
    viewport_.TopLeftY = 0.0F;
    viewport_.Height = (f32) desc.BufferDesc.Height;
    viewport_.Width = (f32) desc.BufferDesc.Width;
    viewport_.MaxDepth = 1.0F;
    viewport_.MinDepth = 0.0F;
    scissorRect_ = { 0, 0, (i32) desc.BufferDesc.Width, (i32) desc.BufferDesc.Height };
}

void Surface::SetViewport(ID3D12GraphicsCommandList *cmdList) {
    cmdList->RSSetViewports(1, &viewport_);
    cmdList->RSSetScissorRects(1, &scissorRect_);
}

void Surface::AllowTearing(IDXGIFactory5* factory) {
    BOOL allowTearing = FALSE;
    if (SUCCEEDED(factory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(u32))) && (allowTearing != 0)) {
        presentInfo_ = DXGI_PRESENT_ALLOW_TEARING;
        swapChainFlags_ |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
    }
}

const window::Resolution &Surface::Resolution() {
    return *resolution_;
}

void Surface::Present() {
    swapChain_->Present(0, presentInfo_) >> utl::DxCheck;
}

void Surface::Resize(const window::Resolution &res) {
    *resolution_ = res;
    swapChain_->ResizeBuffers(frameBufferCount, resolution_->width, resolution_->height, DXGI_FORMAT_R8G8B8A8_UNORM,
                              swapChainFlags_) >> utl::DxCheck;

}

void Surface::GetBuffer(u32 index, ComPtr<ID3D12Resource> &buffer) {
    swapChain_->GetBuffer(index, IID_PPV_ARGS(&buffer)) >> utl::DxCheck;
}

void Surface::SetWindow(WHandle &winHandle) {
    window_ = winHandle;
}

}