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

#include "resources/dx_resources.hpp"
#include "render/mesh.hpp"

namespace reveal3d::graphics::dx {

struct Layer {
    void Set(ID3D12GraphicsCommandList* cmdList);
    GraphicsPso pso;
    RootSignature rootSignature;
};

struct Mesh;

class RenderLayers {
public:
    RenderLayers();
    void BuildRoots(ID3D12Device *device);
    void BuildPSOs(ID3D12Device *device);
    void AddMesh(render::Mesh &mesh);
    void DrawLayer(ID3D12GraphicsCommandList* cmdList, FrameResource& frame, u32 layer);

    INLINE Layer& operator[] (u32 index) { return layers_[index]; }
    INLINE const Layer& operator[] (u32 index) const { return layers_[index]; }
private:


    Layer layers_[render::shader::count];
    std::vector<render::Mesh*> meshes_[render::shader::count];

//    Layer worldGridLayer_;

};

}

