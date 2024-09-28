/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_render_layers.hpp
 * @version 1.0
 * @date 02/04/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "dx_common.hpp"
#include "dx_pso.hpp"
#include "dx_root_signature.hpp"

#include "dx_render_info.hpp"
#include "render/mesh.hpp"
#include "resources/dx_resources.hpp"

namespace reveal3d::graphics::dx12 {

struct Layer {
    void Set(ID3D12GraphicsCommandList* cmdList);
    GraphicsPso pso;
    RootSignature rootSignature;
};


class RenderLayers {
public:
    RenderLayers();
    ~RenderLayers();
    void BuildRoots(ID3D12Device *device);
    void BuildPSOs(ID3D12Device *device);
    void AddMesh(render::SubMesh &mesh);
    void RemoveMesh(render::SubMesh &mesh);
    void DrawLayer(ID3D12GraphicsCommandList* cmdList, FrameResource& frame, std::vector<RenderElement> &elements, u32 layer);
    void DrawEffectLayer(ID3D12GraphicsCommandList* cmdList, u32 layer);

    INLINE Layer& operator[] (u32 index) { return layers_.at(index); }
    INLINE const Layer& operator[] (u32 index) const { return layers_.at(index); }
private:
    std::array<Layer ,render::Shader::count> layers_;
    std::array<std::vector<render::SubMesh *>, render::Shader::count> meshes_;
//    std::vector<RenderElement> renderElements_;
};

}

