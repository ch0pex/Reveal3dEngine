/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_gpass.hpp
 * @version 1.0
 * @date 12/08/2024
 * @brief Short description
 *
 * Longer description
 */
#pragma once

#include "dx_adapter.hpp"
#include "dx_commands.hpp"
#include "dx_common.hpp"
#include "resources/dx_descriptor_heap.hpp"
#include "window/window_info.hpp"

namespace reveal3d::graphics::dx12 {

struct RenderTarget {
  ComPtr<ID3D12Resource> resource_;
  DescriptorHandle rtv_;
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

  ID3D12Resource* back_buffer() { return render_targets_.at(Commands::frameIndex()).resource_.Get(); }

  D3D12_CPU_DESCRIPTOR_HANDLE rtv() { return render_targets_.at(Commands::frameIndex()).rtv_.cpu; }

  [[nodiscard]] window::Resolution resolution() const { return resolution_; };

private:
  void allowTearing(IDXGIFactory5* factory);

  void finalize();

  void getBuffer(u32 index, ComPtr<ID3D12Resource>& buffer) const;

  utl::ResourceArray<RenderTarget> render_targets_;
  window::Resolution resolution_;
  ComPtr<IDXGISwapChain3> swap_chain_;
  u32 swap_chain_flags_;
  u32 present_info_;
  WHandle window_ {};
  D3D12_VIEWPORT viewport_ {};
  D3D12_RECT scissor_rect_ {};
};

} // namespace reveal3d::graphics::dx12
