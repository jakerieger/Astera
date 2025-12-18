#pragma once

#include <string>
#include <unordered_map>

namespace Astera {
    /// @brief Expands macro variables in a string in place.
    ///
    /// Searches for macro patterns in the format `${VariableName}` and replaces
    /// them with corresponding values from the provided variable map. Multiple
    /// occurrences of the same variable are all replaced. Variables not found
    /// in the map are left unchanged in the output string.
    ///
    /// @param[in,out] text The string containing macros to be expanded.
    ///                     Modified in place with all recognized macros replaced.
    /// @param[in] variables A map of variable names to their replacement values.
    ///                      Keys should not include the `${` prefix or `}` suffix.
    ///
    /// @note Nested macros (e.g., `${${inner}}`) are not supported.
    /// @note If a macro has no closing brace, it and all subsequent text is left unchanged.
    ///
    /// @par Example
    /// @code
    /// std::string text = "Hello, ${name}! Welcome to ${place}.";
    /// std::unordered_map<std::string, std::string> vars = {
    ///     {"name", "Alice"},
    ///     {"place", "Wonderland"}
    /// };
    /// ExpandMacros(text, vars);
    /// // text is now "Hello, Alice! Welcome to Wonderland."
    /// @endcode
    ///
    /// @see https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#variable-references
    inline void ExpandMacros(std::string& text, const std::unordered_map<std::string, std::string>& variables) {
        constexpr std::string_view prefix = "${";
        constexpr char suffix             = '}';

        std::size_t pos = 0;
        while ((pos = text.find(prefix, pos)) != std::string::npos) {
            const std::size_t endPos = text.find(suffix, pos + prefix.size());
            if (endPos == std::string::npos) {
                break;  // No closing brace found
            }
            std::string varName = text.substr(pos + prefix.size(), endPos - pos - prefix.size());

            auto it = variables.find(varName);
            if (it != variables.end()) {
                text.replace(pos, endPos - pos + 1, it->second);
                pos += it->second.size();  // Move past the replacement
            } else {
                pos = endPos + 1;  // Skip unknown variables
            }
        }
    }
}  // namespace Astera