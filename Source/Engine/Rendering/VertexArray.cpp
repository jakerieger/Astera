/*
 *  Filename: VertexArray.cpp
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

#include "VertexArray.hpp"
#include "GLUtils.hpp"

namespace Nth {
    VertexArray::VertexArray() {
        GLCall(glGenVertexArrays, 1, &mArrayID);
    }

    VertexArray::~VertexArray() {
        Destroy();
    }

    VertexArray::VertexArray(VertexArray&& other) noexcept
        : mArrayID(std::exchange(other.mArrayID, 0)), mVertexBufferIndex(std::exchange(other.mVertexBufferIndex, 0)),
          mVertexBuffers(std::move(other.mVertexBuffers)), mIndexBuffer(std::move(other.mIndexBuffer)) {}

    VertexArray& VertexArray::operator=(VertexArray&& other) noexcept {
        if (this != &other) {
            if (mArrayID != 0) { GLCall(glDeleteVertexArrays, 1, &mArrayID); }
            mArrayID           = std::exchange(other.mArrayID, 0);
            mVertexBufferIndex = std::exchange(other.mVertexBufferIndex, 0);
            mVertexBuffers     = std::move(other.mVertexBuffers);
            mIndexBuffer       = std::move(other.mIndexBuffer);
        }
        return *this;
    }

    void VertexArray::AddVertexBuffer(const shared_ptr<VertexBuffer>& buffer, const VertexLayout& layout) {
        N_ASSERT(mArrayID != 0);
        N_ASSERT(buffer != nullptr);
        N_ASSERT(!layout.GetAttributes().empty());

        Bind();
        buffer->Bind();

        const auto& attributes = layout.GetAttributes();
        u32 offset             = 0;

        for (const auto& attribute : attributes) {
            GLCall(glEnableVertexAttribArray, mVertexBufferIndex);
            GLCall(glVertexAttribPointer,
                   mVertexBufferIndex,
                   CAST<GLint>(attribute.GetComponentCount()),
                   attribute.GetGLType(),
                   attribute.normalized ? GL_TRUE : GL_FALSE,
                   CAST<GLsizei>(layout.GetStride()),
                   RCAST<void*>(CAST<uintptr_t>(offset)));

            offset += attribute.GetSize();
            mVertexBufferIndex++;
        }

        mVertexBuffers.push_back(buffer);
    }

    void VertexArray::SetIndexBuffer(const shared_ptr<IndexBuffer>& buffer) {
        N_ASSERT(mArrayID != 0);
        N_ASSERT(buffer != nullptr);

        Bind();
        buffer->Bind();
        mIndexBuffer = buffer;
    }

    void VertexArray::Bind() const {
        GLCall(glBindVertexArray, mArrayID);
    }

    void VertexArray::Unbind() {
        GLCall(glBindVertexArray, 0);
    }

    void VertexArray::Destroy() {
        for (const auto& vbo : mVertexBuffers) {
            vbo->Destroy();
        }
        mIndexBuffer->Destroy();

        if (mArrayID != 0) {
            GLCall(glDeleteVertexArrays, 1, &mArrayID);
            mArrayID = 0;
        }
    }
}  // namespace Nth