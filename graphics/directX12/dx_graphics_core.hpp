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
#include "dx_surface.hpp"


#include <array>

namespace reveal3d::graphics {


__declspec(align(16))
class Dx12 {
public:

    explicit Dx12(window::Resolution *res);
    void LoadPipeline();
    void LoadAssets();
    void LoadAsset(u32 id);
    void Update(render::Camera &camera);
    void RenderSurface();
    void Terminate();
    void Resize(const window::Resolution &res);
    INLINE void SetWindow(WHandle& winHandle) { surface_.SetWindow(winHandle); }
    INLINE ID3D12Device* GetDevice() { return device_.Get(); }
    INLINE dx12::Heaps& GetHeaps() { return heaps_;}

private:
    void InitDXGIAdapter();
    void CreateSwapChain();
    void InitFrameResources();
    void InitDsBuffer();
    void InitConstantBuffers();
    void SetViewport();
    void CreateRenderElement(u32 index);
    void RemoveAsset(id_t id);

    /****************** Factory and Device *****************/

    ComPtr<IDXGIFactory5> factory_;
    ComPtr<ID3D12Device> device_;

    /****************** Frame resources, depth stencil and swapchain *****************/

    std::array<dx12::FrameResource, dx12::frameBufferCount> frameResources_;

    /***************** Depth stencil buffer**********************/
    ComPtr<ID3D12Resource> depthStencilBuffer_;
    dx12::DescriptorHandle dsHandle_;

    dx12::Heaps heaps_;
    dx12::Commands cmdManager_;

    /************ Render elements and layers**********/
    std::vector<RenderElement> renderElements_;
    dx12::RenderLayers renderLayers_;

    /***************** Surface Info **********************/
    dx12::Surface surface_;
};

}
