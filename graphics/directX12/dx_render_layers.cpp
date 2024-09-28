/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_render_layers.cpp
 * @version 1.0
 * @date 02/04/2024
 * @brief Short description
 *
 * Longer description
 */

#include "dx_render_layers.hpp"

namespace reveal3d::graphics::dx12 {
/*
RenderLayers::RenderLayers() {
    layers_[render::Shader::opaque].rootSignature.Reset(4);
    layers_[render::Shader::unlit].rootSignature.Reset(4);
    layers_[render::Shader::grid].rootSignature.Reset(4);
//    worldGridLayer_.rootSignature.Reset(1);
}
void RenderLayers::AddMesh(render::SubMesh &mesh) {
    meshes_[mesh.shader].push_back(&mesh);
}

void RenderLayers::RemoveMesh(render::SubMesh &mesh) {
    std::erase_if(meshes_[mesh.shader], [mesh](render::SubMesh *submesh){return std::addressof(mesh) == submesh; });
}

void RenderLayers::DrawLayer(ID3D12GraphicsCommandList *cmdList, FrameResource &frame, std::vector<RenderElement>& elements, u32 layer) {
    for (auto *mesh : meshes_[layer]) {
        if (!mesh->visible) continue;
        cmdList->SetGraphicsRootConstantBufferView(0, frame.constantBuffer.GpuPos(mesh->constantIndex));
        cmdList->SetGraphicsRootConstantBufferView(1, frame.matBuffer.GpuPos(mesh->constantIndex));
        cmdList->IASetVertexBuffers(0, 1, elements.at(mesh->renderInfo).vertexBuffer.View());
        cmdList->IASetIndexBuffer(elements[mesh->renderInfo].indexBuffer.View());
        cmdList->IASetPrimitiveTopology(elements[mesh->renderInfo].topology);
        cmdList->DrawIndexedInstanced(mesh->indexCount, 1, mesh->indexPos, mesh->vertexPos, 0);
    }
}

void RenderLayers::DrawEffectLayer(ID3D12GraphicsCommandList *cmdList, u32 layer) {
    cmdList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    for (auto *mesh : meshes_[layer]) {
        cmdList->DrawInstanced(mesh->indexCount, 1, mesh->indexPos, mesh->vertexPos);
    }

}
RenderLayers::~RenderLayers() {
    delete meshes_[render::Shader::grid].at(0);
}

void Layer::Set(ID3D12GraphicsCommandList *cmdList) {
    cmdList->SetPipelineState(pso.Get());
    cmdList->SetGraphicsRootSignature(rootSignature.Get());
}
*/

}