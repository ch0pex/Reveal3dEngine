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

#include "dx_common.hpp"
#include "window/window_info.hpp"
#include "dx_commands.hpp"

namespace reveal3d::graphics::dx12 {

class Surface {
public:
    Surface(window::Resolution& resolution, u32 swap_chain_flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH, u32 present_info = 0);
    void init(Commands &cmd_manager, IDXGIFactory5 *factory);
    void updateViewport();
    void setViewport(ID3D12GraphicsCommandList *cmd_list);
    void setWindow(WHandle &win_handle);
    void allowTearing(IDXGIFactory5 *factory);
    const window::Resolution&resolution();
    void present();
    void resize(const window::Resolution &res);
    void getBuffer(u32 index, ComPtr<ID3D12Resource> &buffer);

private:
    ComPtr<IDXGISwapChain3> swap_chain_;
    u32 swap_chain_flags_;
    u32 present_info_;

    window::Resolution *resolution_;
    WHandle window_ {};
    D3D12_VIEWPORT viewport_ {};
    D3D12_RECT scissor_rect_{};
};

}
