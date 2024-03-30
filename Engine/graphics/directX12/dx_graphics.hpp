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


#include "core/scene.hpp"
#include "dx_commands.hpp"
#include "dx_common.hpp"
#include "dx_render_info.hpp"
#include "render/camera.hpp"
#include "resources/dx_resources.hpp"
#include "window/window_info.hpp"


#include <array>

namespace reveal3d::graphics::dx {

__declspec(align(16))
class Graphics {
public:
    explicit Graphics(window::Resolution *res);
    void LoadPipeline();
    void LoadAssets(core::Scene &scene);
    void Update(core::Scene &scene, render::Camera &camera, const Timer& timer);
    void PrepareRender();
    void Draw();
    void Terminate();
    void Resize(const window::Resolution &res);
    INLINE void SetWindow(WHandle winHandle) { window_ = winHandle; }
    [[nodiscard]] INLINE u32 GetWidth() const { return resolution_->width; }
    [[nodiscard]] INLINE u32 GetHeight() const { return resolution_->height; }


private:
    void InitDXGIAdapter();
    void CreateSwapChain();
    void InitFrameResources();
    void InitDsBuffer();
    void InitConstantBuffers();
    void SetViewport();
    void BuildRootSignature();
    void BuildPSO();

    /****************** Factory and Device *****************/

    ComPtr<IDXGIFactory5> factory_;
    ComPtr<ID3D12Device> device_;

    /****************** Frame resources, depth stencil and swapchain *****************/
    struct FrameResource {
        ConstantBuffer constantBuffer;
        ComPtr<ID3D12Resource> backBuffer;
        DescriptorHandle backBufferHandle;
        PassCB passBuffer;
        DescriptorHandle passHandle;
    };

    FrameResource frameResources_[frameBufferCount];
    ComPtr<IDXGISwapChain3> swapChain_;

    ComPtr<ID3D12Resource> depthStencilBuffer_;
    DescriptorHandle dsHandle_;
    /***************** Heaps and buffers **********************/
    Heaps heaps_;
    std::vector<RenderInfo> renderElements_;

    /************ Pipeline state and commands manager **********/
    ComPtr<ID3D12RootSignature> rootSignature_;
    ComPtr<ID3D12PipelineState> pipelineState_;
    Commands cmdManager_;

    /***************** Surface Info **********************/
    window::Resolution *resolution_;
    D3D12_VIEWPORT viewport_ {};
    D3D12_RECT scissorRect_ {};
    HWND window_ {};
    u32 swapChainFlags_;
    u32 presentInfo_;
};

}

using reveal3d::graphics::dx::Graphics;