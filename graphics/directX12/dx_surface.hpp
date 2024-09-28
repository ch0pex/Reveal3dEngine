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

#include "dx_common.hpp"
#include "window/window_info.hpp"
#include "dx_commands.hpp"

namespace reveal3d::graphics::dx12 {

class Surface {
public:
    Surface(window::Resolution& resolution, u32 swapChainFlags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH, u32 present_info = 0);
    void Init(Commands& cmdManager, IDXGIFactory5* factory);
    void UpdateViewport();
    void SetViewport(ID3D12GraphicsCommandList *cmdList);
    void SetWindow(WHandle& winHandle);
    void AllowTearing(IDXGIFactory5* factory);
    const window::Resolution& Resolution();
    void Present();
    void Resize(const window::Resolution& res);
    void GetBuffer(u32 index, ComPtr<ID3D12Resource>& buffer);

private:
    ComPtr<IDXGISwapChain3> swapChain_;
    u32 swapChainFlags_;
    u32 presentInfo_;

    window::Resolution *resolution_;
    WHandle window_ {};
    D3D12_VIEWPORT viewport_ {};
    D3D12_RECT scissorRect_ {};
};

}
