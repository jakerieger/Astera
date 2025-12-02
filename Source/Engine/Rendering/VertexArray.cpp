/// @author Jake Rieger
/// @created 12/1/25
///
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