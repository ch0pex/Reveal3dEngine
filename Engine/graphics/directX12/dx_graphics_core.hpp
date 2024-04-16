/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_graphics.hpp
 * @version 1.0
 * @date 25/02/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif


#include "dx_commands.hpp"
#include "dx_common.hpp"
#include "dx_render_info.hpp"
#include "dx_render_layers.hpp"
#include "resources/dx_resources.hpp"

#include "core/scene.hpp"
#include "render/camera.hpp"
#include "window/window_info.hpp"


#include <array>

//using reveal3d::graphics::dx::RenderInfo;

namespace reveal3d::graphics::dx {


__declspec(align(16))
class Graphics {
public:
    explicit Graphics(window::Resolution *res);
    void LoadPipeline();
    void LoadAssets();
    void LoadAsset();
    void Update(render::Camera &camera);
    void PrepareRender();
    void Draw();
    void Terminate();
    void Resize(const window::Resolution &res);
    INLINE void SetWindow(WHandle winHandle) { window_ = winHandle; }

private:
    void InitDXGIAdapter();
    void CreateSwapChain();
    void InitFrameResources();
    void InitDsBuffer();
    void InitConstantBuffers();
    void SetViewport();
    void GetHardwareAdapter( _In_ IDXGIFactory1* pFactory, _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter);
    /****************** Factory and Device *****************/

    ComPtr<IDXGIFactory5> factory_;
    ComPtr<ID3D12Device> device_;

    /****************** Frame resources, depth stencil and swapchain *****************/

    FrameResource frameResources_[frameBufferCount];
    ComPtr<IDXGISwapChain3> swapChain_;

    /***************** Depth stencil buffer**********************/
    ComPtr<ID3D12Resource> depthStencilBuffer_;
    DescriptorHandle dsHandle_;

    Heaps heaps_;
    Commands cmdManager_;

    /************ Render elements and layers**********/
    std::vector<RenderInfo> renderElements_;
    RenderLayers renderLayers_;

    /***************** Surface Info **********************/
    window::Resolution *resolution_;
    D3D12_VIEWPORT viewport_ {};
    D3D12_RECT scissorRect_ {};
    WHandle window_ {};
    u32 swapChainFlags_;
    u32 presentInfo_;
};

}

using reveal3d::graphics::dx::Graphics;