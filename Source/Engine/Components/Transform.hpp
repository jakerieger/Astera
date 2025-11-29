// Author: Jake Rieger
// Created: 11/28/25.
//

#pragma once

#include "CommonPCH.hpp"

namespace Nth {
    struct Transform {
        Vec2 position;
        Vec2 rotation;
        Vec2 scale;

        void Translate(const Vec2& translation) {
            position += translation;
        }
    };
}  // namespace N
