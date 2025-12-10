/*
 *  Filename: ShaderManager.hpp
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

#include "EngineCommon.hpp"
#include "Rendering/Shader.hpp"

namespace Nth {
    /// @brief Predefined shader name constants
    namespace Shaders {
        /// @brief Name identifier for the sprite shader
        inline constexpr std::string_view Sprite = "sprite";

        /// @brief Name identifier for the sprite batch shader
        inline constexpr std::string_view SpriteInstanced = "sprite_instanced";
    }  // namespace Shaders

    /// @brief Manages shader resources and provides cached access to compiled shaders
    class ShaderManager {
    public:
        /// @brief Initializes the shader manager and loads default shaders
        static void Initialize();

        /// @brief Shuts down the shader manager and releases all cached shaders
        static void Shutdown();

        /// @brief Retrieves a shader by name from the cache
        /// @param name The name identifier of the shader to retrieve
        /// @return Shared pointer to the requested shader, or nullptr if not found
        N_ND static shared_ptr<Shader> GetShader(std::string_view name);

    private:
        /// @brief Cache mapping shader names to their compiled shader objects
        static unordered_map<string, shared_ptr<Shader>> sCache;

        /// @brief Singleton instance of the shader manager
        static shared_ptr<ShaderManager> sManager;

        /// @brief Retrieves or creates the singleton shader manager instance
        /// @return Shared pointer to the shader manager instance
        static shared_ptr<ShaderManager> GetManager();
    };
}  // namespace Nth