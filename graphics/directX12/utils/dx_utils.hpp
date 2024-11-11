/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_debugger.hpp
 * @version 1.0
 * @date 27/02/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "common/common.hpp"
#include "config/config.hpp"

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

using namespace DirectX;
using Microsoft::WRL::ComPtr;

namespace reveal3d::graphics::dx12::utl {

template<typename T>
void release(T*& resource) {
  if (resource) {
    resource->Release();
    resource = nullptr;
  }
}

/// This class offers a safe array for that resources that need to be replicated as times as swap chain buffers
template<typename T>
class ResourceArray {
public:
  using container      = std::array<T, config::Render::Graphics::max_buffer_count>;
  using iterator       = typename container::iterator;
  using const_iterator = typename container::const_iterator;

  [[nodiscard]] constexpr const_iterator begin() const { return const_iterator(resource_.begin()); }

  [[nodiscard]] constexpr const_iterator end() const {
    return const_iterator(resource_.begin() + config::render.graphics.buffer_count);
  }

  constexpr iterator begin() { return iterator(resource_.begin()); }

  constexpr iterator end() { return iterator(resource_.begin() + config::render.graphics.buffer_count); }

  constexpr T& at(u8 const index) {
    assert(index < config::render.graphics.buffer_count);
    return resource_.at(index);
  }

private:
  container resource_;
};

void enable_cpu_layer(u32& factory_flag);
void enable_gpu_layer();
void queue_info(ID3D12Device* device, BOOL enable);
void log_adapters();
void set_reporter(ID3D12Device* device);
void get_hardware_adapter(IDXGIFactory1* p_factory, IDXGIAdapter1** pp_adapter);
void report_live_device_objs();
void display_fps();

} // namespace reveal3d::graphics::dx12::utl
