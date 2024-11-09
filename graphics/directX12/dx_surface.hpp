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

#include "dx_commands.hpp"
#include "dx_common.hpp"
#include "window/window_info.hpp"

namespace reveal3d::graphics::dx12 {

class Surface {
public:
  explicit Surface(
      window::Resolution& resolution, u32 swap_chain_flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH,
      u32 present_info = 0
  );
  void init(const Commands& cmd_manager, IDXGIFactory5* factory);
  void updateViewport();
  void setViewport(ID3D12GraphicsCommandList* cmd_list) const;
  void setWindow(const WHandle& win_handle);
  void allowTearing(IDXGIFactory5* factory);
  const window::Resolution& resolution() const;
  void present() const;
  void resize(const window::Resolution& res) const;
  void getBuffer(u32 index, ComPtr<ID3D12Resource>& buffer) const;

private:
  ComPtr<IDXGISwapChain3> swap_chain_;
  u32 swap_chain_flags_;
  u32 present_info_;

  window::Resolution* resolution_;
  WHandle window_ {};
  D3D12_VIEWPORT viewport_ {};
  D3D12_RECT scissor_rect_ {};
};

} // namespace reveal3d::graphics::dx12
