/*
 *  Filename: Content.hpp
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

#ifndef ASTERA_ENGINE_CONTENT_DIR
    #define ASTERA_ENGINE_CONTENT_DIR ""
#endif

#include "EngineCommon.hpp"

namespace Astera {
    /// @brief Type of content the container holds
    enum class ContentType {
        Audio,
        Scene,
        Script,
        Shader,
        Sprite,
    };

    /// @brief Content container and helper class
    class Content {
        /// @brief Root directory for scene files
        static constexpr std::string_view kSceneRoot = "Scenes";
        /// @brief Root directory for script files
        static constexpr std::string_view kScriptRoot = "Scripts";
        /// @brief Root directory for shader files
        static constexpr std::string_view kShaderRoot = "Shaders";
        /// @brief Root directory for sprite files
        static constexpr std::string_view kSpriteRoot = "Sprites";
        /// @brief Root directory for audio files
        static constexpr std::string_view kAudioRoot = "Audio";

        /// @brief Path to the game content root directory
        inline static fs::path sContentRoot;

        /// @brief Path to the engine content root directory.
        ///
        /// Defaults to the internal engine definition. ASTERA_ENGINE_CONTENT_DIR maps to the installed location when
        /// linking against Astera in an external project via find_package().
        inline static fs::path sEngineContentRoot {ASTERA_ENGINE_CONTENT_DIR};

    public:
        /// @brief Retrieves the full path to a content file based on its type
        /// @tparam type The type of content to retrieve
        /// @tparam engine Whether to use the engine content root (default: false)
        /// @param filename The name of the file to retrieve
        /// @return The full filesystem path to the requested content file
        /// @throws std::invalid_argument if an invalid content type is specified
        template<ContentType type, bool engine = false>
        inline static fs::path Get(const string& filename) {
            fs::path contentPath = sContentRoot;
            if constexpr (engine) { contentPath = sEngineContentRoot; }

            if constexpr (type == ContentType::Audio) {
                return contentPath / kAudioRoot / filename;
            } else if constexpr (type == ContentType::Scene) {
                return contentPath / kSceneRoot / filename;
            } else if constexpr (type == ContentType::Script) {
                return contentPath / kScriptRoot / filename;
            } else if constexpr (type == ContentType::Shader) {
                return contentPath / kShaderRoot / filename;
            } else if constexpr (type == ContentType::Sprite) {
                return contentPath / kSpriteRoot / filename;
            } else {
                throw std::invalid_argument("Invalid content type");
            }
        }

        /// @brief Sets the game content root path
        /// @param path The filesystem path to set as the content root
        inline static void SetContentPath(const fs::path& path) {
            sContentRoot = path;
        }

        /// @brief Sets the engine content root path
        /// @param path The filesystem path to set as the engine content root
        inline static void SetEngineContentPath(const fs::path& path) {
            sEngineContentRoot = path;
        }

        /// @brief Initializes content paths to their default runtime locations
        inline static void SetRuntimeDefaults() {
            sContentRoot       = fs::current_path() / "Content";
            sEngineContentRoot = sContentRoot / "EngineContent";
        }
    };
}  // namespace Astera