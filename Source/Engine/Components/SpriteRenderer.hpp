// Author: Jake Rieger
// Created: 11/28/25.
//

#pragma once

#include "CommonPCH.hpp"
#include "Rendering/Geometry.hpp"

namespace Nth {
    struct SpriteRenderer {
        u32 textureId;
        Geometry* geometry;
    };
}  // namespace Nth
