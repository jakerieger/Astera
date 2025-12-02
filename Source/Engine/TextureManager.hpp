/// @author Jake Rieger
/// @created 11/28/25
///

#pragma once

#include "CommonPCH.hpp"

namespace Nth {
    using TextureID = u32;

    class TextureManager {
    public:
        static void Initialize();
        static void Shutdown();

        static TextureID GetTextureByName(const string& name);
        static TextureID Load(const fs::path& filename);

    private:
        static unordered_map<string, TextureID> sCache;
        static shared_ptr<TextureManager> sManager;
        static shared_ptr<TextureManager> GetManager();
    };
}  // namespace N
