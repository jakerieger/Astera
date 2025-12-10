/*
 *  Filename: Buffer.cpp
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

#include "Buffer.hpp"
#include "GLUtils.hpp"

namespace Astera {
#pragma region IBuffer
    IBuffer::IBuffer() {
        GLCall(glGenBuffers, 1, &mBufferID);
    }

    IBuffer::~IBuffer() {
        Destroy();
    }

    IBuffer::IBuffer(IBuffer&& other) noexcept
        : mBufferID(std::exchange(other.mBufferID, 0)), mSize(std::exchange(other.mSize, 0)) {}

    IBuffer& IBuffer::operator=(IBuffer&& other) noexcept {
        if (this != &other) {
            if (mBufferID != 0) { GLCall(glDeleteBuffers, 1, &mBufferID); }
            mBufferID = std::exchange(other.mBufferID, 0);
            mSize     = std::exchange(other.mSize, 0);
        }
        return *this;
    }

    void IBuffer::Destroy() {
        if (mBufferID != 0) {
            GLCall(glDeleteBuffers, 1, &mBufferID);
            mBufferID = 0;
        }
    }
#pragma endregion

#pragma region VertexBuffer
    void VertexBuffer::SetData(const void* data, size_t size, BufferUsage usage) {
        N_ASSERT(mBufferID != 0);
        Bind();
        GLCall(glBufferData, GL_ARRAY_BUFFER, CAST<GLsizeiptr>(size), data, CAST<GLenum>(usage));
        mSize = size;
    }

    void VertexBuffer::UpdateData(const void* data, size_t size, size_t offset) {
        N_ASSERT(mBufferID != 0);
        N_ASSERT(offset + size <= mSize);
        Bind();
        GLCall(glBufferSubData, GL_ARRAY_BUFFER, CAST<GLintptr>(offset), CAST<GLsizeiptr>(size), data);
    }

    void VertexBuffer::Bind() {
        GLCall(glBindBuffer, GL_ARRAY_BUFFER, mBufferID);
    }

    void VertexBuffer::Unbind() {
        GLCall(glBindBuffer, GL_ARRAY_BUFFER, 0);
    }
#pragma endregion

#pragma region IndexBuffer
    void IndexBuffer::SetData(const void* data, size_t size, BufferUsage usage) {
        N_ASSERT(mBufferID != 0);
        Bind();
        GLCall(glBufferData, GL_ELEMENT_ARRAY_BUFFER, CAST<GLsizeiptr>(size), data, CAST<GLenum>(usage));
        mSize = size;
        // Note: size is in bytes, count needs to be calculated externally or use SetIndices()
    }

    void IndexBuffer::UpdateData(const void* data, size_t size, size_t offset) {
        N_ASSERT(mBufferID != 0);
        N_ASSERT(offset + size <= mSize);
        Bind();
        GLCall(glBufferSubData, GL_ELEMENT_ARRAY_BUFFER, CAST<GLintptr>(offset), CAST<GLsizeiptr>(size), data);
    }

    void IndexBuffer::SetIndices(const u32* indices, size_t count, BufferUsage usage) {
        N_ASSERT(mBufferID != 0);
        N_ASSERT(indices != nullptr);

        mCount            = count;
        const size_t size = count * sizeof(u32);

        Bind();
        GLCall(glBufferData, GL_ELEMENT_ARRAY_BUFFER, CAST<GLsizeiptr>(size), indices, CAST<GLenum>(usage));
        mSize = size;
    }

    void IndexBuffer::Bind() {
        GLCall(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, mBufferID);
    }

    void IndexBuffer::Unbind() {
        GLCall(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, 0);
    }
#pragma endregion
}  // namespace Astera