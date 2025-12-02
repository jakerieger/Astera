/// @author Jake Rieger
/// @created 12/1/25
///

#pragma once

#include "CommonPCH.hpp"

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
