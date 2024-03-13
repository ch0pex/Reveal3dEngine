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


#include "dx_common.hpp"
#include "dx_commands.hpp"
#include "resources/dx_resources.hpp"
#include "render/camera.hpp"
#include "window/window_info.hpp"

#include <array>

namespace reveal3d::graphics::dx {

class Graphics {
public:
    explicit Graphics(const window::Resolution &res);
    void LoadPipeline();
    void LoadAssets();
    void Update(render::Camera &camera);
    void PopulateCommands();
    void Draw();
    void Terminate();
    INLINE void SetWindow(WHandle winHandle) { window_ = winHandle; }
    [[nodiscard]] INLINE u32 GetWidth() const { return resolution_.width; }
    [[nodiscard]] INLINE u32 GetHeight() const { return resolution_.height; }


private:
    void InitDXGIAdapter();
    void CreateSwapChain();
    void InitFrameResources();
    void SetViewport();

    /****************** Factory and Device *****************/

    ComPtr<IDXGIFactory5> factory_;
    ComPtr<ID3D12Device> device_;
    ComPtr<IDXGISwapChain3> swapChain_;

    /***************** Resources **********************/
    struct FrameResource {
        ComPtr<ID3D12Resource> resource;
        DescriptorHandle handle;
    };

    FrameResource backBuffers_[frameBufferCount];
    //FrameResource depthStencilBuffer;

    //ComPtr<ID3D12RootSignature> rootSignature_;
    //ComPtr<ID3D12PipelineState> pipelineState_;

    // App resources.
    //ComPtr<ID3D12Resource> vertexBuffer_;
    //D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

    /************ Commands and heaps managers **********/

    Heaps heapsManager_;
    Commands cmdManager_;

    /***************** Window Info **********************/

    D3D12_VIEWPORT viewport_;
    D3D12_RECT scissorRect_;
    window::Resolution resolution_;
    HWND window_;
};

}

using reveal3d::graphics::dx::Graphics;