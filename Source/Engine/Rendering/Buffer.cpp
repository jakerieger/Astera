/// @author Jake Rieger
/// @created 12/1/25
///
#include "Buffer.hpp"
#include "GLUtils.hpp"

namespace Nth {
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
}  // namespace Nth