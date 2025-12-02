/// @author Jake Rieger
/// @created 11/30/25
///

#pragma once

#include "CommonPCH.hpp"

namespace Nth::Content {
    inline constexpr std::string_view ContentRoot = "Content";

    inline fs::path GetContentPath(const fs::path& filename) {
        return fs::current_path() / ContentRoot / filename;
    }
}  // namespace Nth::Content
