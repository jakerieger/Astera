/*
 *  Filename: Command.hpp
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

#include <variant>

#include "EngineCommon.hpp"
#include "Components/SpriteRenderer.hpp"
#include "Components/Transform.hpp"

namespace Nth {
    /// @brief Command to clear the framebuffer
    struct ClearCommand {
        Vec4 color {0.0f, 0.0f, 0.0f, 1.0f};
        bool clearDepth {true};
        bool clearStencil {false};
    };

    /// @brief Command to draw a sprite/quad
    struct DrawSpriteCommand {
        SpriteRenderer sprite;
        Transform transform;
        Vec2 screenDimensions;
        Vec4 tintColor {1.0f, 1.0f, 1.0f, 1.0f};
    };

    /// @brief Command to set the viewport
    struct SetViewportCommand {
        i32 x {0};
        i32 y {0};
        u32 width {800};
        u32 height {600};
    };

    /// @brief Command to bind a shader program
    struct BindShaderCommand {
        u32 programId {0};
    };

    /// @brief Command to set a uniform value
    struct SetUniformCommand {
        u32 programId {0};
        string name;
        std::variant<i32, f32, Vec2, Vec3, Vec4, Mat4> value;
    };

    // ============================================================================
    // Core Drawing Commands
    // ============================================================================

    /// @brief Draw geometry using indexed triangles
    struct DrawIndexedCommand {
        shared_ptr<VertexArray> vao;
        u32 indexCount;
        GLenum primitiveType {GL_TRIANGLES};
        u32 indexOffset {0};
    };

    /// @brief Draw geometry using instanced indexed triangles
    struct DrawIndexedInstancedCommand {
        shared_ptr<VertexArray> vao;
        u32 indexCount;
        u32 instanceCount;
        GLenum primitiveType {GL_TRIANGLES};
        u32 indexOffset {0};
    };

    /// @brief Draw geometry without indices (vertex arrays only)
    struct DrawArraysCommand {
        shared_ptr<VertexArray> vao;
        u32 vertexCount;
        u32 vertexOffset {0};
        GLenum primitiveType {GL_TRIANGLES};
    };

    // ============================================================================
    // Buffer Management Commands
    // ============================================================================

    /// @brief Update vertex buffer data
    struct UpdateVertexBufferCommand {
        shared_ptr<VertexBuffer> buffer;
        vector<u8> data;
        size_t offset {0};
    };

    /// @brief Update index buffer data
    struct UpdateIndexBufferCommand {
        shared_ptr<IndexBuffer> buffer;
        vector<u32> indices;
        size_t offset {0};
    };

    /// @brief Bind a vertex array object
    struct BindVertexArrayCommand {
        shared_ptr<VertexArray> vao;
    };

    /// @brief Unbind the currently bound vertex array
    struct UnbindVertexArrayCommand {};

    /// @brief Per-instance data for sprite batching
    struct SpriteInstanceData {
        Mat4 transform;  ///< Model-view-projection matrix for this sprite
        Vec4 tintColor;  ///< Color tint (for future use)

        SpriteInstanceData() = default;
        SpriteInstanceData(const Mat4& transform, const Vec4& tintColor = Vec4(1.0f))
            : transform(transform), tintColor(tintColor) {}
    };

    /// @brief A batch of sprites sharing the same texture
    struct SpriteBatch {
        u32 textureId;
        vector<SpriteInstanceData> instances;
        shared_ptr<VertexArray> quadVAO;  ///< Shared quad geometry

        void Clear() {
            instances.clear();
        }

        N_ND size_t SpriteCount() const {
            return instances.size();
        }
    };

    /// @brief Variant type that can hold any command
    using RenderCommand = std::variant<ClearCommand,
                                       DrawSpriteCommand,
                                       SetViewportCommand,
                                       BindShaderCommand,
                                       SetUniformCommand,
                                       DrawIndexedCommand,
                                       DrawIndexedInstancedCommand,
                                       DrawArraysCommand,
                                       UpdateVertexBufferCommand,
                                       UpdateIndexBufferCommand,
                                       BindVertexArrayCommand,
                                       UnbindVertexArrayCommand>;
}  // namespace Nth
