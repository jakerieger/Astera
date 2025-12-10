/*
 *  Filename: VertexArray.hpp
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
#include "Buffer.hpp"

namespace Nth {
    /// @brief Describes the data type of a vertex attribute
    enum class AttributeType {
        Float,
        Float2,
        Float3,
        Float4,
        Int,
        Int2,
        Int3,
        Int4,
        Boolean,
    };

    /// @brief Describes a single vertex attribute (position, color, texcoord, etc.)
    struct VertexAttribute {
        string name;
        AttributeType type;
        bool normalized;

        VertexAttribute(string name, AttributeType type, bool normalized = false)
            : name(std::move(name)), type(type), normalized(normalized) {}

        /// @brief Get the component count for this attribute type
        N_ND u32 GetComponentCount() const {
            switch (type) {
                case AttributeType::Float:
                case AttributeType::Int:
                case AttributeType::Boolean:
                    return 1;
                case AttributeType::Float2:
                case AttributeType::Int2:
                    return 2;
                case AttributeType::Float3:
                case AttributeType::Int3:
                    return 3;
                case AttributeType::Float4:
                case AttributeType::Int4:
                    return 4;
            }
            return 0;
        }

        /// @brief Get the size in bytes for this attribute type
        N_ND u32 GetSize() const {
            switch (type) {
                case AttributeType::Float:
                    return sizeof(f32);
                case AttributeType::Float2:
                    return sizeof(f32) * 2;
                case AttributeType::Float3:
                    return sizeof(f32) * 3;
                case AttributeType::Float4:
                    return sizeof(f32) * 4;
                case AttributeType::Int:
                    return sizeof(i32);
                case AttributeType::Int2:
                    return sizeof(i32) * 2;
                case AttributeType::Int3:
                    return sizeof(i32) * 3;
                case AttributeType::Int4:
                    return sizeof(i32) * 4;
                case AttributeType::Boolean:
                    return sizeof(bool);
            }
            return 0;
        }

        /// @brief Get the OpenGL type for this attribute
        N_ND GLenum GetGLType() const {
            switch (type) {
                case AttributeType::Float:
                case AttributeType::Float2:
                case AttributeType::Float3:
                case AttributeType::Float4:
                    return GL_FLOAT;
                case AttributeType::Int:
                case AttributeType::Int2:
                case AttributeType::Int3:
                case AttributeType::Int4:
                    return GL_INT;
                case AttributeType::Boolean:
                    return GL_BOOL;
            }
            return 0;
        }
    };

    /// @brief Describes the layout of vertex data
    class VertexLayout {
    public:
        VertexLayout() = default;

        /// @brief Add an attribute to the layout
        void AddAttribute(const VertexAttribute& attribute) {
            mAttributes.push_back(attribute);
            mStride += attribute.GetSize();
        }

        N_ND const vector<VertexAttribute>& GetAttributes() const {
            return mAttributes;
        }

        N_ND u32 GetStride() const {
            return mStride;
        }

    private:
        vector<VertexAttribute> mAttributes;
        u32 mStride {0};
    };

    /// @brief Abstraction for OpenGL Vertex Array Objects (VAOs)
    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        N_CLASS_PREVENT_COPIES(VertexArray)

        VertexArray(VertexArray&& other) noexcept;
        VertexArray& operator=(VertexArray&& other) noexcept;

        /// @brief Add a vertex buffer with its layout to this VAO
        /// @param buffer The vertex buffer to add
        /// @param layout The layout describing the vertex buffer's data
        void AddVertexBuffer(const shared_ptr<VertexBuffer>& buffer, const VertexLayout& layout);

        /// @brief Set the index buffer for this VAO
        /// @param buffer The index buffer to use
        void SetIndexBuffer(const shared_ptr<IndexBuffer>& buffer);

        /// @brief Bind this vertex array
        void Bind() const;

        /// @brief Unbind the currently bound vertex array
        static void Unbind();

        N_ND const vector<shared_ptr<VertexBuffer>>& GetVertexBuffers() const {
            return mVertexBuffers;
        }

        N_ND const shared_ptr<IndexBuffer>& GetIndexBuffer() const {
            return mIndexBuffer;
        }

        N_ND GLuint GetID() const {
            return mArrayID;
        }

        void Destroy();

    private:
        GLuint mArrayID {0};
        u32 mVertexBufferIndex {0};
        vector<shared_ptr<VertexBuffer>> mVertexBuffers;
        shared_ptr<IndexBuffer> mIndexBuffer;
    };
}  // namespace Nth