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

#include "render/mesh.hpp"

namespace reveal3d::graphics::dx {

struct Layer {
    GraphicsPso pso;
    RootSignature rootSignature;
};

class RenderLayers {
public:
    RenderLayers();
    void Build(ID3D12Device *device);
private:
    Layer layers_[render::shader::count];
    std::vector<RenderInfo *> renderElements[render::shader::count];

    Layer worldGridLayer_;

};

}

