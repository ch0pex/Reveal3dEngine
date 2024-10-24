//
// Created by acbsu on 13/08/2024.
//

#include "dx_surface.hpp"

namespace reveal3d::graphics::dx12 {

Surface::Surface(window::Resolution &resolution, u32 swap_chain_flags, u32 present_info)
        : resolution_(&resolution), swap_chain_flags_(swap_chain_flags), present_info_(present_info)
{

}

void Surface::init(Commands &cmd_manager, IDXGIFactory5 *factory) {
    ComPtr<IDXGISwapChain1> swap_chain_1;

    const DXGI_SWAP_CHAIN_DESC1 swap_chain_desc{
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
            .Flags = swap_chain_flags_};
    factory->CreateSwapChainForHwnd(cmd_manager.getQueue(), window_.hwnd, &swap_chain_desc, nullptr, nullptr, &swap_chain_1) >> utl::DxCheck;
    factory->MakeWindowAssociation(window_.hwnd, DXGI_MWA_NO_ALT_ENTER) >> utl::DxCheck; // Disable Alt + Enter for full screen window
    swap_chain_1.As(&swap_chain_) >> utl::DxCheck;
}

void Surface::updateViewport() {
    DXGI_SWAP_CHAIN_DESC desc {};

    swap_chain_->GetDesc(&desc)  >> utl::DxCheck;
    viewport_.TopLeftX = 0.0F;
    viewport_.TopLeftY = 0.0F;
    viewport_.Height = (f32) desc.BufferDesc.Height;
    viewport_.Width = (f32) desc.BufferDesc.Width;
    viewport_.MaxDepth = 1.0F;
    viewport_.MinDepth = 0.0F;
    scissor_rect_ = { 0, 0, (i32) desc.BufferDesc.Width, (i32) desc.BufferDesc.Height };
}

void Surface::setViewport(ID3D12GraphicsCommandList *cmd_list) {
    cmd_list->RSSetViewports(1, &viewport_);
    cmd_list->RSSetScissorRects(1, &scissor_rect_);
}

void Surface::allowTearing(IDXGIFactory5 *factory) {
    BOOL allow_tearing = FALSE;
    if (SUCCEEDED(factory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allow_tearing, sizeof(u32))) && (allow_tearing != 0)) {
        present_info_ = DXGI_PRESENT_ALLOW_TEARING;
        swap_chain_flags_ |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
    }
}

const window::Resolution &Surface::resolution() {
    return *resolution_;
}

void Surface::present() { swap_chain_->Present(0, present_info_) >> utl::DxCheck; }

void Surface::resize(const window::Resolution &res) {
    *resolution_ = res;
    swap_chain_->ResizeBuffers(frameBufferCount, resolution_->width, resolution_->height, DXGI_FORMAT_R8G8B8A8_UNORM,
                               swap_chain_flags_) >> utl::DxCheck;

}

void Surface::getBuffer(u32 index, ComPtr<ID3D12Resource> &buffer) {
    swap_chain_->GetBuffer(index, IID_PPV_ARGS(&buffer)) >> utl::DxCheck;
}

void Surface::setWindow(WHandle &win_handle) {
    window_ = win_handle;
}

}