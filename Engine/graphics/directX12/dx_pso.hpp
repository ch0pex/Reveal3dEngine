/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_pso.hpp
 * @version 1.0
 * @date 02/04/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "dx_common.hpp"
#include "dx_root_signature.hpp"

namespace reveal3d::graphics::dx {

class GraphicsPso {
public:
    GraphicsPso();
    void Finalize(ID3D12Device *device);


private:
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    RootSignature* rootSignature_;
    ComPtr<ID3D12PipelineState> pipelineState_;
};

}

