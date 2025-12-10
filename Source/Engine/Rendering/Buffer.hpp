/*
 *  Filename: Buffer.hpp
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

namespace Nth {
    enum class BufferUsage {
        Static  = GL_STATIC_DRAW,
        Dynamic = GL_DYNAMIC_DRAW,
        Stream  = GL_STREAM_DRAW,
    };

    /// @brief Base buffer interface inherited by classes like VertexBuffer and IndexBuffer
    class IBuffer {
    public:
        IBuffer();
        virtual ~IBuffer();

        N_CLASS_PREVENT_COPIES(IBuffer)

        IBuffer(IBuffer&& other) noexcept;
        IBuffer& operator=(IBuffer&& other) noexcept;

        /// @brief Upload vertex data to the GPU
        /// @param data Pointer to vertex data
        /// @param size Size of data in bytes
        /// @param usage Buffer usage pattern
        virtual void SetData(const void* data, size_t size, BufferUsage usage) = 0;

        /// @brief Update a portion of the buffer data
        /// @param data Pointer to new data
        /// @param size Size of data in bytes
        /// @param offset Offset in bytes from start of buffer
        virtual void UpdateData(const void* data, size_t size, size_t offset) = 0;

        /// @brief Bind this vertex buffer
        virtual void Bind() = 0;

        /// @brief Unbind the currently bound vertex buffer
        virtual void Unbind() = 0;

        N_ND GLuint GetID() const {
            return mBufferID;
        }

        N_ND size_t GetSize() const {
            return mSize;
        }

        void Destroy();

    protected:
        GLuint mBufferID {0};
        size_t mSize {0};
    };

    class VertexBuffer final : public IBuffer {
    public:
        void SetData(const void* data, size_t size, BufferUsage usage) override;
        void UpdateData(const void* data, size_t size, size_t offset) override;
        void Bind() override;
        void Unbind() override;
    };

    class IndexBuffer final : public IBuffer {
    public:
        void SetData(const void* data, size_t size, BufferUsage usage) override;
        void UpdateData(const void* data, size_t size, size_t offset) override;
        void Bind() override;
        void Unbind() override;

        void SetIndices(const u32* indices, size_t count, BufferUsage usage);

        N_ND size_t GetCount() const {
            return mCount;
        }

    private:
        size_t mCount {0};
    };
}  // namespace Nth
