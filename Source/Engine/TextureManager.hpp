/*
 *  Filename: TextureManager.hpp
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