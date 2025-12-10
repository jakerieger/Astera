/*
 *  Filename: PhysicsDebugLayer.hpp
 *  This code is part of the Nth Engine core library
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
#include "DebugInterface.hpp"
#include "Color.hpp"
#include "Components/Transform.hpp"

namespace Nth {
    class PhysicsDebugLayer final : public IDebugOverlay {
    public:
        PhysicsDebugLayer(u32 width, u32 height);
        ~PhysicsDebugLayer() override;

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnEvent(const Event& event) override;

        void UpdateTransforms(const vector<Transform>& transforms) {
            mTransforms = transforms;
        }

    private:
        vector<Transform> mTransforms;

        u32 mWidth;
        u32 mHeight;

        GLuint mShaderProgram {0};
        GLuint mVAO {0};
        GLuint mVBO {0};
        GLint mColorLocation {0};

        void InitShaders();
        void SetupBuffers();
        void DrawVertices(const vector<f32>& vertices, GLenum mode) const;

        N_ND f32 ScreenToClipX(f32 x) const;
        N_ND f32 ScreenToClipY(f32 y) const;

        void DrawLine(f32 x0, f32 y0, f32 x1, f32 y1, const Color& color) const;
        void DrawLine(const Vec2& start, const Vec2& end, const Color& color) const;
        void DrawRectangle(f32 x, f32 y, f32 width, f32 height, const Color& color, bool filled = true) const;
        void DrawCircle(f32 x, f32 y, f32 radius, u32 segments, const Color& color, bool filled = true) const;
        void DrawPolygon(const vector<Vec2>& points, const Color& color, bool filled = true) const;

        inline static constexpr std::string_view kVertexShaderSource = R""(#version 460 core
layout (location = 0) in vec2 aPos;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
}
    )"";

        inline static constexpr std::string_view kFragmentShaderSource = R""(#version 460 core
out vec4 FragColor;
uniform vec4 uColor;

void main() {
    FragColor = uColor;
}
    )"";
    };
}  // namespace Nth
