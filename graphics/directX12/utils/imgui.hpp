/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file imgui.hpp
 * @version 1.0
 * @date 17/01/2025
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include <backends/imgui_impl_dx12.h>
#include <d3d12.h>
#include <imgui.h>

namespace reveal3d::graphics::dx12 {

inline void imgui_start(ID3D12GraphicsCommandList* command_list, ID3D12DescriptorHeap* srv_heap_descriptor) {
#ifdef IMGUI
  command_list->SetDescriptorHeaps(1, &srv_heap_descriptor);
  ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), command_list);
#endif
}

inline void imgui_end(ID3D12GraphicsCommandList* command_list) {
#ifdef IMGUI
  ImGui::UpdatePlatformWindows();
  ImGui::RenderPlatformWindowsDefault(nullptr, command_list);
#endif
}

} // namespace reveal3d::graphics::dx12
