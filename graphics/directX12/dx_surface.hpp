/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_surface.hpp
 * @version 1.0
 * @date 12/08/2024
 * @brief Short description
 *
 * Longer description
 */
#pragma once


#include "dx_commands.hpp"
#include "resources/dx_descriptor_heap.hpp"
#include "utils/dx_debug.hpp"
#include "utils/dx_resource_array.hpp"
#include "window/window_info.hpp"


namespace reveal3d::graphics::dx12 {

constexpr D3D12_RENDER_TARGET_VIEW_DESC rtv_default_desc = {
  .Format = DXGI_FORMAT_R8G8B8A8_UNORM, .ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D
};

class RenderTarget {
public:
  using init_info = D3D12_RENDER_TARGET_VIEW_DESC;

  RenderTarget() = default;

  explicit RenderTarget(DescriptorHandle const& handle, init_info const& desc, ID3D12Resource* buffer) :
    buf_(buffer), rtv_(handle) {
    adapter.device->CreateRenderTargetView(buf_.Get(), &desc, rtv_.cpu);
  }

  [[nodiscard]] auto const& handle() const { return rtv_; }

  [[nodiscard]] auto resource() const { return buf_.Get(); }

  void resetBuffer(ID3D12Resource* buffer) {
    buf_.Reset();
    buf_ = buffer;
  }

private:
  ComPtr<ID3D12Resource> buf_ {};
  DescriptorHandle rtv_ {};
};

class Surface {
public:
  explicit Surface(
      window::Resolution const& resolution, u32 swap_chain_flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH,
      u32 present_info = 0
  );

  void createSwapChain(Commands const& cmd_manager, Heaps& heaps);

  void setViewport(ID3D12GraphicsCommandList* cmd_list) const;

  void setWindow(WHandle const& win_handle);

  void present() const;

  void resize(window::Resolution const& res);

  ID3D12Resource* back_buffer() { return render_targets_.at(Commands::frameIndex()).resource(); }

  D3D12_CPU_DESCRIPTOR_HANDLE rtv() { return render_targets_.at(Commands::frameIndex()).handle().cpu; }

  [[nodiscard]] window::Resolution resolution() const { return resolution_; };

private:
  void allowTearing(IDXGIFactory5* factory);

  void finalize();


  utils::ResourceArray<RenderTarget> render_targets_;
  window::Resolution resolution_;
  ComPtr<IDXGISwapChain3> swap_chain_;
  u32 swap_chain_flags_;
  u32 present_info_;
  WHandle window_ {};
  D3D12_VIEWPORT viewport_ {};
  D3D12_RECT scissor_rect_ {};
};

} // namespace reveal3d::graphics::dx12
