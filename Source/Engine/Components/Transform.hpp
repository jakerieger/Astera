/// @author Jake Rieger
/// @created 11/28/25
///

#pragma once

#include "Common/CommonPCH.hpp"

namespace Nth {
    struct Transform {
        Vec2 position;
        Vec2 rotation;
        Vec2 scale {1, 1};

        void Translate(const Vec2& translation) {
            position += translation;
        }

        void Rotate(f32 angle) {
            rotation.x += angle;
        }

        void Scale(const Vec2& scaleFactor) {
            scale *= scaleFactor;
        }

        N_ND Mat4 GetMatrix() const {
            auto mat = Mat4(1.0f);
            mat      = glm::translate(mat, glm::vec3(position.x, position.y, 0.0f));
            mat = glm::rotate(mat, glm::radians(rotation.x), glm::vec3(0.0f, 0.0f, 1.0f));  // Z-axis rotation for 2D
            mat = glm::scale(mat, glm::vec3(scale.x, scale.y, 1.0f));
            return mat;
        }
    };
}  // namespace Nth
