// Author: Jake Rieger
// Created: 12/10/25.
//

#pragma once

#include "CommonPCH.hpp"
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
