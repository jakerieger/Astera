/*
 *  Filename: StringConvert.inl
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
 */

#pragma once

#include "EngineCommon.hpp"
#include <array>

namespace Astera {
    namespace detail {

        /// @brief Trim leading whitespace from string_view
        ASTERA_KEEP constexpr std::string_view TrimLeading(std::string_view sv) noexcept {
            while (!sv.empty() &&
                   (sv.front() == ' ' || sv.front() == '\t' || sv.front() == '\n' || sv.front() == '\r')) {
                sv.remove_prefix(1);
            }
            return sv;
        }

        /// @brief Generic integer parsing using std::from_chars
        template<typename T>
            requires std::is_integral_v<T>
        ASTERA_KEEP Result<T> ParseInteger(std::string_view str, int base = 10) noexcept {
            str = TrimLeading(str);

            if (str.empty()) {
                return unexpected("Empty input string");
            }

            T result {};
            const char* first = str.data();
            const char* last  = first + str.size();

            auto [ptr, ec] = std::from_chars(first, last, result, base);

            if (ec == std::errc::invalid_argument) {
                return unexpected("Invalid integer format");
            }
            if (ec == std::errc::result_out_of_range) {
                return unexpected("Integer value out of range");
            }
            if (ptr != last) {
                std::string_view remaining(ptr, CAST<std::size_t>(last - ptr));
                remaining = TrimLeading(remaining);
                if (!remaining.empty()) {
                    return unexpected("Trailing characters in string");
                }
            }

            return result;
        }

        /// @brief Generic floating-point parsing using std::from_chars
        template<typename T>
            requires std::is_floating_point_v<T>
        ASTERA_KEEP Result<T> ParseFloat(std::string_view str,
                                         std::chars_format fmt = std::chars_format::general) noexcept {
            str = TrimLeading(str);

            if (str.empty()) {
                return unexpected("Empty input string");
            }

            T result {};
            const char* first = str.data();
            const char* last  = first + str.size();

            auto [ptr, ec] = std::from_chars(first, last, result, fmt);

            if (ec == std::errc::invalid_argument) {
                return unexpected("Invalid floating-point format");
            }
            if (ec == std::errc::result_out_of_range) {
                return unexpected("Floating-point value out of range");
            }
            if (ptr != last) {
                std::string_view remaining(ptr, CAST<std::size_t>(last - ptr));
                remaining = TrimLeading(remaining);
                if (!remaining.empty()) {
                    return unexpected("Trailing characters in string");
                }
            }

            return result;
        }

        /// @brief Maximum buffer sizes for to_chars conversions
        template<typename T>
        constexpr std::size_t MaxCharsSize() noexcept {
            if constexpr (std::is_same_v<T, i32>) {
                return 12;  // -2147483648
            } else if constexpr (std::is_same_v<T, u32>) {
                return 11;  // 4294967295
            } else if constexpr (std::is_same_v<T, i64>) {
                return 21;  // -9223372036854775808
            } else if constexpr (std::is_same_v<T, u64>) {
                return 21;  // 18446744073709551615
            } else if constexpr (std::is_same_v<T, f32>) {
                return 48;
            } else if constexpr (std::is_same_v<T, f64>) {
                return 328;
            } else {
                return 64;
            }
        }

    }  // namespace detail

    // ============================================================================
    // String to Number Conversions
    // ============================================================================

    /// @brief Parse string to i32
    /// @param str Input string (may have leading/trailing whitespace)
    /// @param base Numeric base (2-36, default 10)
    /// @return Result containing the value or error message
    ASTERA_KEEP inline Result<i32> StringToI32(std::string_view str, int base = 10) noexcept {
        return detail::ParseInteger<i32>(str, base);
    }

    /// @brief Parse string to u32
    /// @param str Input string (may have leading/trailing whitespace)
    /// @param base Numeric base (2-36, default 10)
    /// @return Result containing the value or error message
    ASTERA_KEEP inline Result<u32> StringToU32(std::string_view str, int base = 10) noexcept {
        return detail::ParseInteger<u32>(str, base);
    }

    /// @brief Parse string to i64
    /// @param str Input string (may have leading/trailing whitespace)
    /// @param base Numeric base (2-36, default 10)
    /// @return Result containing the value or error message
    ASTERA_KEEP inline Result<i64> StringToI64(std::string_view str, int base = 10) noexcept {
        return detail::ParseInteger<i64>(str, base);
    }

    /// @brief Parse string to u64
    /// @param str Input string (may have leading/trailing whitespace)
    /// @param base Numeric base (2-36, default 10)
    /// @return Result containing the value or error message
    ASTERA_KEEP inline Result<u64> StringToU64(std::string_view str, int base = 10) noexcept {
        return detail::ParseInteger<u64>(str, base);
    }

    /// @brief Parse string to f32
    /// @param str Input string (may have leading/trailing whitespace)
    /// @param fmt Format specifier (general, scientific, fixed, hex)
    /// @return Result containing the value or error message
    ASTERA_KEEP inline Result<f32> StringToF32(std::string_view str,
                                               std::chars_format fmt = std::chars_format::general) noexcept {
        return detail::ParseFloat<f32>(str, fmt);
    }

    /// @brief Parse string to f64
    /// @param str Input string (may have leading/trailing whitespace)
    /// @param fmt Format specifier (general, scientific, fixed, hex)
    /// @return Result containing the value or error message
    ASTERA_KEEP inline Result<f64> StringToF64(std::string_view str,
                                               std::chars_format fmt = std::chars_format::general) noexcept {
        return detail::ParseFloat<f64>(str, fmt);
    }

    // ============================================================================
    // Number to String Conversions
    // ============================================================================

    /// @brief Convert i32 to string
    /// @param value The value to convert
    /// @param base Numeric base (2-36, default 10)
    /// @return String representation
    ASTERA_KEEP inline string ToString(i32 value, int base = 10) {
        std::array<char, detail::MaxCharsSize<i32>()> buffer;
        auto [ptr, ec] = std::to_chars(buffer.data(), buffer.data() + buffer.size(), value, base);
        return string(buffer.data(), ptr);
    }

    /// @brief Convert u32 to string
    /// @param value The value to convert
    /// @param base Numeric base (2-36, default 10)
    /// @return String representation
    ASTERA_KEEP inline string ToString(u32 value, int base = 10) {
        std::array<char, detail::MaxCharsSize<u32>()> buffer;
        auto [ptr, ec] = std::to_chars(buffer.data(), buffer.data() + buffer.size(), value, base);
        return string(buffer.data(), ptr);
    }

    /// @brief Convert i64 to string
    /// @param value The value to convert
    /// @param base Numeric base (2-36, default 10)
    /// @return String representation
    ASTERA_KEEP inline string ToString(i64 value, int base = 10) {
        std::array<char, detail::MaxCharsSize<i64>()> buffer;
        auto [ptr, ec] = std::to_chars(buffer.data(), buffer.data() + buffer.size(), value, base);
        return string(buffer.data(), ptr);
    }

    /// @brief Convert u64 to string
    /// @param value The value to convert
    /// @param base Numeric base (2-36, default 10)
    /// @return String representation
    ASTERA_KEEP inline string ToString(u64 value, int base = 10) {
        std::array<char, detail::MaxCharsSize<u64>()> buffer;
        auto [ptr, ec] = std::to_chars(buffer.data(), buffer.data() + buffer.size(), value, base);
        return string(buffer.data(), ptr);
    }

    /// @brief Convert f32 to string
    /// @param value The value to convert
    /// @param fmt Format specifier (general, scientific, fixed, hex)
    /// @return String representation
    ASTERA_KEEP inline string ToString(f32 value, std::chars_format fmt = std::chars_format::general) {
        std::array<char, detail::MaxCharsSize<f32>()> buffer;
        auto [ptr, ec] = std::to_chars(buffer.data(), buffer.data() + buffer.size(), value, fmt);
        return string(buffer.data(), ptr);
    }

    /// @brief Convert f32 to string with precision
    /// @param value The value to convert
    /// @param precision Number of digits
    /// @param fmt Format specifier
    /// @return String representation
    ASTERA_KEEP inline string ToString(f32 value, int precision, std::chars_format fmt = std::chars_format::general) {
        std::array<char, detail::MaxCharsSize<f32>()> buffer;
        auto [ptr, ec] = std::to_chars(buffer.data(), buffer.data() + buffer.size(), value, fmt, precision);
        return string(buffer.data(), ptr);
    }

    /// @brief Convert f64 to string
    /// @param value The value to convert
    /// @param fmt Format specifier (general, scientific, fixed, hex)
    /// @return String representation
    ASTERA_KEEP inline string ToString(f64 value, std::chars_format fmt = std::chars_format::general) {
        std::array<char, detail::MaxCharsSize<f64>()> buffer;
        auto [ptr, ec] = std::to_chars(buffer.data(), buffer.data() + buffer.size(), value, fmt);
        return string(buffer.data(), ptr);
    }

    /// @brief Convert f64 to string with precision
    /// @param value The value to convert
    /// @param precision Number of digits
    /// @param fmt Format specifier
    /// @return String representation
    ASTERA_KEEP inline string ToString(f64 value, int precision, std::chars_format fmt = std::chars_format::general) {
        std::array<char, detail::MaxCharsSize<f64>()> buffer;
        auto [ptr, ec] = std::to_chars(buffer.data(), buffer.data() + buffer.size(), value, fmt, precision);
        return string(buffer.data(), ptr);
    }

    // ============================================================================
    // Convenience Functions (with default values)
    // ============================================================================

    /// @brief Parse string to i32, returning default on failure
    ASTERA_KEEP inline i32 StringToI32Or(std::string_view str, i32 defaultValue, int base = 10) noexcept {
        return StringToI32(str, base).value_or(defaultValue);
    }

    /// @brief Parse string to u32, returning default on failure
    ASTERA_KEEP inline u32 StringToU32Or(std::string_view str, u32 defaultValue, int base = 10) noexcept {
        return StringToU32(str, base).value_or(defaultValue);
    }

    /// @brief Parse string to i64, returning default on failure
    ASTERA_KEEP inline i64 StringToI64Or(std::string_view str, i64 defaultValue, int base = 10) noexcept {
        return StringToI64(str, base).value_or(defaultValue);
    }

    /// @brief Parse string to u64, returning default on failure
    ASTERA_KEEP inline u64 StringToU64Or(std::string_view str, u64 defaultValue, int base = 10) noexcept {
        return StringToU64(str, base).value_or(defaultValue);
    }

    /// @brief Parse string to f32, returning default on failure
    ASTERA_KEEP inline f32
    StringToF32Or(std::string_view str, f32 defaultValue, std::chars_format fmt = std::chars_format::general) noexcept {
        return StringToF32(str, fmt).value_or(defaultValue);
    }

    /// @brief Parse string to f64, returning default on failure
    ASTERA_KEEP inline f64
    StringToF64Or(std::string_view str, f64 defaultValue, std::chars_format fmt = std::chars_format::general) noexcept {
        return StringToF64(str, fmt).value_or(defaultValue);
    }

    // ============================================================================
    // Validation Helpers
    // ============================================================================

    /// @brief Check if string is a valid i32 representation
    ASTERA_KEEP inline bool IsValidI32(std::string_view str, int base = 10) noexcept {
        return StringToI32(str, base).has_value();
    }

    /// @brief Check if string is a valid u32 representation
    ASTERA_KEEP inline bool IsValidU32(std::string_view str, int base = 10) noexcept {
        return StringToU32(str, base).has_value();
    }

    /// @brief Check if string is a valid i64 representation
    ASTERA_KEEP inline bool IsValidI64(std::string_view str, int base = 10) noexcept {
        return StringToI64(str, base).has_value();
    }

    /// @brief Check if string is a valid u64 representation
    ASTERA_KEEP inline bool IsValidU64(std::string_view str, int base = 10) noexcept {
        return StringToU64(str, base).has_value();
    }

    /// @brief Check if string is a valid f32 representation
    ASTERA_KEEP inline bool IsValidF32(std::string_view str,
                                       std::chars_format fmt = std::chars_format::general) noexcept {
        return StringToF32(str, fmt).has_value();
    }

    /// @brief Check if string is a valid f64 representation
    ASTERA_KEEP inline bool IsValidF64(std::string_view str,
                                       std::chars_format fmt = std::chars_format::general) noexcept {
        return StringToF64(str, fmt).has_value();
    }

}  // namespace Astera