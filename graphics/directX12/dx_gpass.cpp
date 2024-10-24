//
// Created by acbsu on 12/08/2024.
//

#include "dx_gpass.hpp"

#include "core/scene.hpp"
#include "config/config.hpp"


namespace reveal3d::graphics::dx12 {

const f32 Gpass::clear_color_[] = { config::clearColor.x, config::clearColor.y, config::clearColor.z, config::clearColor.w };

Gpass::Gpass() {
    root_signatures_[render::Shader::Opaque].reset(4);
    root_signatures_[render::Shader::Unlit].reset(4);
    root_signatures_[render::Shader::Grid].reset(4);
}

void Gpass::init(ID3D12Device *device) {
    buildRoots(device);
    buildPsos(device);
}

void Gpass::setRenderTargets(Commands &command_mng, FrameResource &frame_resource) {
    ID3D12GraphicsCommandList*command_list = command_mng.list();

    command_list->ClearRenderTargetView(frame_resource.back_buffer_handle.cpu, clear_color_, 0, nullptr);
    command_list->ClearDepthStencilView(frame_resource.depth_buffer_handle.cpu, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
    command_list->OMSetRenderTargets(1, &frame_resource.back_buffer_handle.cpu, TRUE, &frame_resource.depth_buffer_handle.cpu);

}

void Gpass::render(ID3D12GraphicsCommandList *command_list, FrameResource &frame_resource) {
    curr_root_signature_ = nullptr;
    curr_pipeline_state_ = nullptr;

    command_list->ClearRenderTargetView(frame_resource.back_buffer_handle.cpu, clear_color_, 0, nullptr);
    command_list->ClearDepthStencilView(frame_resource.depth_buffer_handle.cpu, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
    command_list->OMSetRenderTargets(1, &frame_resource.back_buffer_handle.cpu, TRUE, &frame_resource.depth_buffer_handle.cpu);

    for (auto&render_element: render_elements_) {

        auto geometry = render_element.entity.component<core::Geometry>();
        auto transform = render_element.entity.component<core::Transform>();

        for (auto &submesh : geometry.subMeshes()) {
            if (!submesh.visible) continue;

            if (curr_root_signature_ != root_signatures_.at(submesh.shader).get()) {
                curr_root_signature_ = root_signatures_.at(submesh.shader).get();
                command_list->SetGraphicsRootSignature(curr_root_signature_);
            }

            if (curr_pipeline_state_ != pipeline_states_.at(submesh.shader).get()) {
                curr_pipeline_state_ = pipeline_states_.at(submesh.shader).get();
                command_list->SetPipelineState(curr_pipeline_state_);
            }

            // TODO This is the most expensive way to set RootParameters limit 64 DWORDS
            command_list->SetGraphicsRootConstantBufferView(0, frame_resource.constant_buffer.gpuPos(id::index(transform.id()))); // 2 DWORDS
            command_list->SetGraphicsRootConstantBufferView(1, frame_resource.mat_buffer.gpuPos(id::index(geometry.id()))); // 2 DWORDS
            command_list->SetGraphicsRootConstantBufferView(2, frame_resource.pass_buffer.gpuStart()); // 2 DWORDS

            command_list->IASetVertexBuffers(0, 1, render_element.vertex_buffer.view());
            command_list->IASetIndexBuffer(render_element.index_buffer.view());
            command_list->IASetPrimitiveTopology(render_element.topology);
            command_list->DrawIndexedInstanced(submesh.index_count, 1, submesh.index_pos, submesh.vertex_pos, 0);
        }
    }
    drawWorldGrid(command_list, frame_resource);
}

void Gpass::drawWorldGrid(ID3D12GraphicsCommandList *command_list, FrameResource &frame_resource) {
    command_list->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    command_list->SetGraphicsRootSignature(root_signatures_[render::Shader::Grid].get());
    command_list->SetPipelineState(pipeline_states_[render::Shader::Grid].get());
    command_list->SetGraphicsRootConstantBufferView(2, frame_resource.pass_buffer.gpuStart());
    command_list->DrawInstanced(6, 1, 0, 0);
}

//NOTE change geometry to mesh when content module update
void Gpass::addRenderElement(core::Entity entity, Commands &cmd_mng, ID3D12Device *device) {
    core::Geometry geometry = entity.component<core::Geometry>();
    BufferInitInfo vertex_buffer_info = {
            .device = device,
            .cmd_list = cmd_mng.list(),
            .data = geometry.vertices().data(),
            .count = geometry.vertexCount()
    };

    BufferInitInfo index_buffer_info = {
            .device = device,
            .cmd_list = cmd_mng.list(),
            .data = geometry.indices().data(),
            .count = geometry.indexCount(),
            .format = DXGI_FORMAT_R32_UINT
    };

    render_elements_.emplace_back(entity, vertex_buffer_info, index_buffer_info);
}

void Gpass::removeRenderElement(u32 idx) { render_elements_.unordered_remove(idx);
}

void Gpass::terminate() {
    for (auto &elem : render_elements_) {
        elem.vertex_buffer.release();
        elem.index_buffer.release();
    }
}

void Gpass::buildPsos(ID3D12Device *device) {
    ComPtr<ID3DBlob> vertex_shader;
    ComPtr<ID3DBlob> pixel_shader;
    ComPtr<ID3DBlob> errors;

#if defined(_DEBUG)
    // Enable better Shader debugging with the graphics debugging tools.
    UINT compile_flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    UINT compile_flags = 0;
#endif
    HRESULT hr = S_OK;

    //TODO Config file for assets path
    hr = D3DCompileFromFile(relative(L"../../Assets/shaders/hlsl/OpaqueShader.hlsl").c_str(), nullptr, nullptr, "VS", "vs_5_0", compile_flags, 0, &vertex_shader, &errors);
    if (errors != nullptr)
        logger(logDEBUG) << (char *) errors->GetBufferPointer();
    hr >> utl::DxCheck;
    hr = D3DCompileFromFile(relative(L"../../Assets/shaders/hlsl/OpaqueShader.hlsl").c_str(), nullptr, nullptr, "PS", "ps_5_0", compile_flags, 0, &pixel_shader, &errors);
    if (errors != nullptr)
        logger(logDEBUG) << (char *) errors->GetBufferPointer();
    hr >> utl::DxCheck;

    D3D12_INPUT_ELEMENT_DESC input_element_descs[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    pipeline_states_[render::Shader::Opaque].setInputLayout(input_element_descs, _countof(input_element_descs));
    pipeline_states_[render::Shader::Opaque].setRootSignature(root_signatures_[render::Shader::Opaque]);
    pipeline_states_[render::Shader::Opaque].setShaders(vertex_shader.Get(), pixel_shader.Get());
    pipeline_states_[render::Shader::Opaque].setRasterizerCullMode(D3D12_CULL_MODE_NONE);
    pipeline_states_[render::Shader::Opaque].setBlendState(CD3DX12_BLEND_DESC(D3D12_DEFAULT));
    pipeline_states_[render::Shader::Opaque].setDepthStencil(CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT));
    pipeline_states_[render::Shader::Opaque].setSampleMask(UINT_MAX);
    pipeline_states_[render::Shader::Opaque].setPrimitive(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
    pipeline_states_[render::Shader::Opaque].setNumRenderTargets(1U);
    pipeline_states_[render::Shader::Opaque].setRtvFormats(0U, DXGI_FORMAT_R8G8B8A8_UNORM);
    pipeline_states_[render::Shader::Opaque].setSampleDescCount(1U); //TODO: Support x4
    pipeline_states_[render::Shader::Opaque].setDsvFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);

    pipeline_states_[render::Shader::Opaque].finalize(device);

    //TODO Config file for assets path
    hr = D3DCompileFromFile(relative(L"../../Assets/shaders/hlsl/FlatShader.hlsl").c_str(), nullptr, nullptr, "VS", "vs_5_0", compile_flags, 0, &vertex_shader, &errors);
    if (errors != nullptr)
        logger(logDEBUG) << (char *) errors->GetBufferPointer();
    hr >> utl::DxCheck;
    hr = D3DCompileFromFile(relative(L"../../Assets/shaders/hlsl/FlatShader.hlsl").c_str(), nullptr, nullptr, "PS", "ps_5_0", compile_flags, 0, &pixel_shader, &errors);
    if (errors != nullptr)
        logger(logDEBUG) << (char *) errors->GetBufferPointer();
    hr >> utl::DxCheck;

    D3D12_INPUT_ELEMENT_DESC flat_elements_desc[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    pipeline_states_[render::Shader::Unlit].setInputLayout(flat_elements_desc, _countof(flat_elements_desc));
    pipeline_states_[render::Shader::Unlit].setRootSignature(root_signatures_[render::Shader::Opaque]);
    pipeline_states_[render::Shader::Unlit].setShaders(vertex_shader.Get(), pixel_shader.Get());
    pipeline_states_[render::Shader::Unlit].setRasterizerCullMode(D3D12_CULL_MODE_NONE);
    pipeline_states_[render::Shader::Unlit].setBlendState(CD3DX12_BLEND_DESC(D3D12_DEFAULT));
    pipeline_states_[render::Shader::Unlit].setDepthStencil(CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT));
    pipeline_states_[render::Shader::Unlit].setSampleMask(UINT_MAX);
    pipeline_states_[render::Shader::Unlit].setPrimitive(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
    pipeline_states_[render::Shader::Unlit].setNumRenderTargets(1U);
    pipeline_states_[render::Shader::Unlit].setRtvFormats(0U, DXGI_FORMAT_R8G8B8A8_UNORM);
    pipeline_states_[render::Shader::Unlit].setSampleDescCount(1U); //TODO: Support x4
    pipeline_states_[render::Shader::Unlit].setDsvFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);

    pipeline_states_[render::Shader::Unlit].finalize(device);

    hr = D3DCompileFromFile(relative(L"../../Assets/shaders/hlsl/GridShader.hlsl").c_str(), nullptr, nullptr, "VS", "vs_5_0", compile_flags, 0, &vertex_shader, &errors);
    if (errors != nullptr)
        logger(logDEBUG) << (char *) errors->GetBufferPointer();
    hr >> utl::DxCheck;
    hr = D3DCompileFromFile(relative(L"../../Assets/shaders/hlsl/GridShader.hlsl").c_str(), nullptr, nullptr, "PS", "ps_5_0", compile_flags, 0, &pixel_shader, &errors);
    if (errors != nullptr)
        logger(logDEBUG) << (char *) errors->GetBufferPointer();
    hr >> utl::DxCheck;

    D3D12_RENDER_TARGET_BLEND_DESC transparency_blend_desc;
    transparency_blend_desc.BlendEnable = true;
    transparency_blend_desc.LogicOpEnable = false;
    transparency_blend_desc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
    transparency_blend_desc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    transparency_blend_desc.BlendOp = D3D12_BLEND_OP_ADD;
    transparency_blend_desc.SrcBlendAlpha = D3D12_BLEND_ONE;
    transparency_blend_desc.DestBlendAlpha = D3D12_BLEND_ZERO;
    transparency_blend_desc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
    transparency_blend_desc.LogicOp = D3D12_LOGIC_OP_NOOP;
    transparency_blend_desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    D3D12_BLEND_DESC blend_desc = {
            .RenderTarget = transparency_blend_desc};

    pipeline_states_[render::Shader::Grid].setInputLayout(flat_elements_desc, _countof(flat_elements_desc));
    pipeline_states_[render::Shader::Grid].setRootSignature(root_signatures_[render::Shader::Grid]);
    pipeline_states_[render::Shader::Grid].setShaders(vertex_shader.Get(), pixel_shader.Get());
    pipeline_states_[render::Shader::Grid].setRasterizerCullMode(D3D12_CULL_MODE_NONE);
    pipeline_states_[render::Shader::Grid].setBlendState(blend_desc);
    pipeline_states_[render::Shader::Grid].setDepthStencil(CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT));
    pipeline_states_[render::Shader::Grid].setSampleMask(UINT_MAX);
    pipeline_states_[render::Shader::Grid].setPrimitive(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
    pipeline_states_[render::Shader::Grid].setNumRenderTargets(1U);
    pipeline_states_[render::Shader::Grid].setRtvFormats(0U, DXGI_FORMAT_R8G8B8A8_UNORM);
    pipeline_states_[render::Shader::Grid].setSampleDescCount(1U); //TODO: Support x4
    pipeline_states_[render::Shader::Grid].setDsvFormat(DXGI_FORMAT_D24_UNORM_S8_UINT);

    pipeline_states_[render::Shader::Grid].finalize(device);

//    auto gridMesh = new render::SubMesh();
//    gridMesh->indexCount = 6;
//    meshes_[render::Shader::grid].push_back(gridMesh);
}

void Gpass::buildRoots(ID3D12Device *device) {
    for (auto& root : root_signatures_) {
        root[0].InitAsConstantBufferView(0);
        root[1].InitAsConstantBufferView(1);
        root[2].InitAsConstantBufferView(2);
        root[3].InitAsShaderResourceView(3);
        root.finalize(device);

    }
}


}