//
// Created by acbsu on 13/08/2024.
//

#include "dx_surface.hpp"

#include "dx_adapter.hpp"

#include <ranges>

namespace reveal3d::graphics::dx12 {

Surface::Surface(window::Resolution const& resolution, u32 const swap_chain_flags, u32 const present_info) :
  resolution_(resolution), swap_chain_flags_(swap_chain_flags), present_info_(present_info) { }

void Surface::createSwapChain(Commands const& cmd_manager, Heaps& heaps) {
  ComPtr<IDXGISwapChain1> swap_chain_1;

  allowTearing(adapter.factory.Get());

  const DXGI_SWAP_CHAIN_DESC1 swap_chain_desc {
    .Width       = resolution_.width,
    .Height      = resolution_.height,
    .Format      = DXGI_FORMAT_R8G8B8A8_UNORM,
    .Stereo      = FALSE,
    .SampleDesc  = {1, 0},
    .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
    .BufferCount = config::render.graphics.buffer_count,
    .Scaling     = DXGI_SCALING_STRETCH,
    .SwapEffect  = DXGI_SWAP_EFFECT_FLIP_DISCARD,
    .AlphaMode   = DXGI_ALPHA_MODE_UNSPECIFIED,
    .Flags       = swap_chain_flags_
  };


  adapter.factory->CreateSwapChainForHwnd(
      cmd_manager.getQueue(), window_.hwnd, &swap_chain_desc, nullptr, nullptr, &swap_chain_1
  ) >> utl::DxCheck;
  // Disable Alt + Enter for full screen window
  adapter.factory->MakeWindowAssociation(window_.hwnd, DXGI_MWA_NO_ALT_ENTER) >> utl::DxCheck;
  swap_chain_1.As(&swap_chain_) >> utl::DxCheck;

  for (auto [idx, frame_resource]: std::views::enumerate(render_targets_)) {
    frame_resource.rtv_ = heaps.rtv.alloc();
  }

  finalize();
}

void Surface::finalize() {

  constexpr D3D12_RENDER_TARGET_VIEW_DESC rtv_desc = {
    .Format = DXGI_FORMAT_R8G8B8A8_UNORM, .ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D
  };
  for (auto [idx, target]: std::views::enumerate(render_targets_)) {
    getBuffer(idx, target.resource_);
    adapter.device->CreateRenderTargetView(target.resource_.Get(), &rtv_desc, target.rtv_.cpu);
    std::wstring name = L"BackBuffer " + std::to_wstring(idx);
    target.resource_->SetName(name.c_str()) >> utl::DxCheck;
  }

  DXGI_SWAP_CHAIN_DESC desc {};

  swap_chain_->GetDesc(&desc) >> utl::DxCheck;
  viewport_.TopLeftX = 0.0F;
  viewport_.TopLeftY = 0.0F;
  viewport_.Height   = static_cast<f32>(desc.BufferDesc.Height);
  viewport_.Width    = static_cast<f32>(desc.BufferDesc.Width);
  viewport_.MaxDepth = 1.0F;
  viewport_.MinDepth = 0.0F;
  scissor_rect_      = {0, 0, static_cast<i32>(desc.BufferDesc.Width), static_cast<i32>(desc.BufferDesc.Height)};
}

void Surface::setViewport(ID3D12GraphicsCommandList* cmd_list) const {
  cmd_list->RSSetViewports(1, &viewport_);
  cmd_list->RSSetScissorRects(1, &scissor_rect_);
}

void Surface::allowTearing(IDXGIFactory5* factory) {
  BOOL allow_tearing = FALSE;
  if (SUCCEEDED(factory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allow_tearing, sizeof(u32))) &&
      (allow_tearing != 0)) {
    present_info_ = DXGI_PRESENT_ALLOW_TEARING;
    swap_chain_flags_ |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
  }
}

window::Resolution const& Surface::resolution() const { return resolution_; }

void Surface::present() const { swap_chain_->Present(0, present_info_) >> utl::DxCheck; }

void Surface::resize(window::Resolution const& res) {
  for (auto& render_target: render_targets_) {
    render_target.resource_.Reset();
  }
  resolution_ = res;
  swap_chain_->ResizeBuffers(
      config::render.graphics.buffer_count, resolution_.width, resolution_.height, DXGI_FORMAT_R8G8B8A8_UNORM,
      swap_chain_flags_
  ) >> utl::DxCheck;

  finalize();
}

void Surface::getBuffer(u32 const index, ComPtr<ID3D12Resource>& buffer) const {
  swap_chain_->GetBuffer(index, IID_PPV_ARGS(&buffer)) >> utl::DxCheck;
}

void Surface::setWindow(WHandle const& win_handle) { window_ = win_handle; }

} // namespace reveal3d::graphics::dx12
