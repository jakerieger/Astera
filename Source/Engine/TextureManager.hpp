/// @author Jake Rieger
/// @created 11/28/25
///

#pragma once

#include "Common/CommonPCH.hpp"

namespace Nth {
    /// @brief Type alias for texture identifiers
    using TextureID = u32;

    /// @brief Manages texture resources and provides cached access to loaded textures
    class TextureManager {
    public:
        /// @brief Initializes the texture manager
        static void Initialize();

        /// @brief Shuts down the texture manager and releases all cached textures
        static void Shutdown();

        /// @brief Retrieves a texture ID by its name from the cache
        /// @param name The name identifier of the texture to retrieve
        /// @return The texture ID, or an invalid ID if not found
        static TextureID GetTextureByName(const string& name);

        /// @brief Loads a texture from a file and adds it to the cache
        /// @param filename Path to the texture file to load
        /// @return The texture ID for the loaded texture
        static TextureID Load(const fs::path& filename);

    private:
        /// @brief Cache mapping texture names to their texture IDs
        static unordered_map<string, TextureID> sCache;

        /// @brief Singleton instance of the texture manager
        static shared_ptr<TextureManager> sManager;

        /// @brief Retrieves or creates the singleton texture manager instance
        /// @return Shared pointer to the texture manager instance
        static shared_ptr<TextureManager> GetManager();
    };
}  // namespace Nth