/*
 *  Filename: BinaryWriter.hpp
 *  Copyright 2025
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
 */

#pragma once

#include "EngineCommon.hpp"

namespace Astera {
    /// @brief Utility class for writing binary file formats with automatic memory management
    ///
    /// Provides type-safe methods for writing fundamental types, strings, and raw data to
    /// a binary buffer with configurable endianness. Eliminates manual memcpy operations
    /// and pointer arithmetic when creating custom binary file formats.
    class BinaryWriter {
    public:
        /// @brief Byte order for multi-byte values
        enum class Endian {
            Little,  ///< Little endian byte order
            Big,     ///< Big endian byte order
            Native   ///< Native system byte order
        };

        /// @brief Constructs a binary writer with specified initial capacity and endianness
        /// @param initialCapacity Initial buffer size in bytes
        /// @param endian Byte order for multi-byte values (default: Little)
        explicit BinaryWriter(size_t initialCapacity = 1_KB, Endian endian = Endian::Little);

        // Write fundamental types

        /// @brief Writes a signed 8-bit integer
        void WriteInt8(i8 value);

        /// @brief Writes an unsigned 8-bit integer
        void WriteUInt8(u8 value);

        /// @brief Writes a signed 16-bit integer with endian conversion
        void WriteInt16(i16 value);

        /// @brief Writes an unsigned 16-bit integer with endian conversion
        void WriteUInt16(u16 value);

        /// @brief Writes a signed 32-bit integer with endian conversion
        void WriteInt32(i32 value);

        /// @brief Writes an unsigned 32-bit integer with endian conversion
        void WriteUInt32(u32 value);

        /// @brief Writes a signed 64-bit integer with endian conversion
        void WriteInt64(i64 value);

        /// @brief Writes an unsigned 64-bit integer with endian conversion
        void WriteUInt64(u64 value);

        /// @brief Writes a 32-bit floating point value with endian conversion
        void WriteFloat(f32 value);

        /// @brief Writes a 64-bit floating point value with endian conversion
        void WriteDouble(f64 value);

        /// @brief Writes a boolean value as a single byte (0 or 1)
        void WriteBool(bool value);

        // Write strings

        /// @brief Writes a null-terminated C-style string
        /// @param str Null-terminated string to write
        void WriteCString(const char* str);

        /// @brief Writes a string with a 32-bit length prefix
        /// @param str String to write
        void WriteString(const std::string& str);

        /// @brief Writes a fixed-length string, padding with zeros if necessary
        /// @param str String to write
        /// @param fixedLength Total length to write (pads or truncates as needed)
        void WriteFixedString(const std::string& str, size_t fixedLength);

        // Write raw data

        /// @brief Writes raw bytes from a pointer
        /// @param data Pointer to data
        /// @param size Number of bytes to write
        void WriteBytes(const u8* data, size_t size);

        /// @brief Writes raw bytes from a vector
        /// @param data Vector of bytes to write
        void WriteBytes(const std::vector<u8>& data);

        /// @brief Writes an array of values with endian conversion
        /// @tparam T Fundamental type to write
        /// @param data Pointer to array
        /// @param count Number of elements
        template<typename T>
        void WriteArray(const T* data, size_t count) {
            static_assert(std::is_fundamental<T>::value, "WriteArray only works with fundamental types");
            for (size_t i = 0; i < count; ++i) {
                WriteWithEndian(data[i]);
            }
        }

        // Padding and alignment

        /// @brief Writes zero bytes for padding
        /// @param count Number of zero bytes to write
        void WritePadding(size_t count);

        /// @brief Aligns the write position to a boundary
        /// @param alignment Alignment boundary in bytes (e.g., 4, 8, 16)
        void AlignTo(size_t alignment);

        // Position management

        /// @brief Gets the current write position
        /// @return Current position in bytes
        size_t Tell() const;

        /// @brief Seeks to a specific position for writing
        /// @param position Position in bytes
        void Seek(size_t position);

        /// @brief Updates a value at a specific position without changing current position
        /// @tparam T Fundamental type to update
        /// @param position Position in bytes
        /// @param value New value to write
        template<typename T>
        void UpdateAt(size_t position, T value) {
            static_assert(std::is_fundamental<T>::value, "UpdateAt only works with fundamental types");

            if (position + sizeof(T) > mBuffer.size()) {
                throw std::out_of_range("Update position out of range");
            }

            T convertedValue = ConvertEndian(value);
            std::memcpy(mBuffer.data() + position, &convertedValue, sizeof(T));
        }

        // Buffer access

        /// @brief Gets the internal buffer
        /// @return Reference to the byte vector
        const std::vector<u8>& GetBuffer() const;

        /// @brief Gets a raw pointer to the buffer data
        /// @return Pointer to buffer data
        const u8* Data() const;

        /// @brief Gets the current buffer size
        /// @return Size in bytes
        size_t Size() const;

        /// @brief Clears the buffer and resets write position
        void Clear();

        // File operations

        /// @brief Saves the buffer to a file
        /// @param filename Path to the output file
        /// @return True if successful, false otherwise
        bool SaveToFile(const fs::path& filename) const;

        // Endianness control

        /// @brief Sets the byte order for multi-byte values
        /// @param endian New byte order
        void SetEndian(Endian endian);

        /// @brief Gets the current byte order setting
        /// @return Current byte order
        Endian GetEndian() const;

    private:
        std::vector<u8> mBuffer;
        size_t mWritePos {0};
        Endian mEndian;

        /// @brief Writes raw data to the buffer
        void WriteRaw(const void* data, size_t size);

        /// @brief Checks if the system is little endian
        static bool IsLittleEndian();

        /// @brief Converts a value based on endianness setting
        template<typename T>
        T ConvertEndian(T value) {
            if (mEndian == Endian::Native) {
                return value;
            }

            bool needSwap =
              (mEndian == Endian::Little && !IsLittleEndian()) || (mEndian == Endian::Big && IsLittleEndian());

            if (!needSwap || sizeof(T) == 1) {
                return value;
            }

            return SwapBytes(value);
        }

        /// @brief Swaps the byte order of a value
        template<typename T>
        T SwapBytes(T value) {
            static_assert(std::is_fundamental<T>::value, "SwapBytes only works with fundamental types");

            union {
                T value;
                u8 bytes[sizeof(T)];
            } src, dst;

            src.value = value;

            for (size_t i = 0; i < sizeof(T); ++i) {
                dst.bytes[i] = src.bytes[sizeof(T) - 1 - i];
            }

            return dst.value;
        }

        /// @brief Writes a value with endian conversion
        template<typename T>
        void WriteWithEndian(T value) {
            T convertedValue = ConvertEndian(value);
            WriteRaw(&convertedValue, sizeof(T));
        }
    };
}  // namespace Astera