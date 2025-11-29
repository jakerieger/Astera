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

        N_ND Mat4 GetMatrix() const {
            auto mat = Mat4(1.0f);
            mat      = glm::scale(mat, {scale.x, scale.y, 1.0f});
            mat      = glm::rotate(mat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            mat      = glm::rotate(mat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            mat      = glm::translate(mat, glm::vec3(position.x, position.y, 1.0f));
            return mat;
        }
    };
}  // namespace Nth
