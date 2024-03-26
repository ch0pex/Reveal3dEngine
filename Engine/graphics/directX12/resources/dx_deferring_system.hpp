/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dx_resources.hpp
 * @version 1.0
 * @date 13/03/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "../dx_common.hpp"

namespace reveal3d::graphics::dx {

struct Heaps;

void SetDeferredFlag();
void DeferredRelease(IUnknown* resource);
void CleanDeferredResources(Heaps& heaps);

}

