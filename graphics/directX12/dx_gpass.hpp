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

#include "common/common.hpp"
#include "core/components/geometry.hpp"
#include "core/scene.hpp"
#include "dx_commands.hpp"
#include "dx_common.hpp"
#include "dx_pso.hpp"
#include "dx_render_info.hpp"
#include "resources/dx_resources.hpp"

namespace reveal3d::graphics::dx12 {

class Gpass {
public:
    Gpass();
    void Init(ID3D12Device* device);
    void SetRenderTargets(Commands& commandMng, FrameResource& frameResource);
    void Depth_prepass(); //TODO
    void Render(ID3D12GraphicsCommandList* commandList, FrameResource& frameResource);
    void AddRenderElement(core::Entity entity, Commands& cmdMng, ID3D12Device* device);
    void RemoveRenderElement(core::Geometry geometry);
    void Terminate();

private:
    void BuildPsos(ID3D12Device* device);
    void BuildRoots(ID3D12Device* device);
    void DrawWorldGrid(ID3D12GraphicsCommandList* commandList, FrameResource& frameResource);

    static const f32 clearColor_[];

    /**************** Render elements *****************/
    reveal3d::utl::vector<RenderElement>  renderElements_;

    /**************** Pipeline state and root signatures *****************/
    std::array<GraphicsPso, render::Shader::count>    pipelineStates_;
    std::array<RootSignature, render::Shader::count>  rootSignatures_;

    ID3D12RootSignature* currRootSignature_ { nullptr };
    ID3D12PipelineState* currPipelineState_ { nullptr };
};


}