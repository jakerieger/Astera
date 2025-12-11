/*
 *  Filename: Transform.hpp
 *  This code is part of the Astera core library
 *  Copyright 2025 Jake Rieger
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  In no event and under no legal theory, whether in tort (including negligence),
 *  contract, or otherwise, unless required by applicable law (such as deliberate
 *  and grossly negligent acts) or agreed to in writing, shall any Contributor be
 *  liable for any damages, including any direct, indirect, special, incidental,
 *  or consequential damages of any character arising as a result of this License or
 *  out of the use or inability to use the software (including but not limited to damages
 *  for loss of goodwill, work stoppage, computer failure or malfunction, or any and
 *  all other commercial damages or losses), even if such Contributor has been advised
 *  of the possibility of such damages.
 */

#pragma once

#include "EngineCommon.hpp"

namespace Astera {
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

        ASTERA_KEEP Mat4 GetMatrix() const {
            auto mat = Mat4(1.0f);
            mat      = glm::translate(mat, glm::vec3(position.x, position.y, 0.0f));
            mat = glm::rotate(mat, glm::radians(rotation.x), glm::vec3(0.0f, 0.0f, 1.0f));  // Z-axis rotation for 2D
            mat = glm::scale(mat, glm::vec3(scale.x, scale.y, 1.0f));
            return mat;
        }
    };
}  // namespace Astera
