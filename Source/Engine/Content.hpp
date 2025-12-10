/// @author Jake Rieger
/// @created 11/30/25
///

#pragma once

#include "Common/CommonPCH.hpp"

namespace Nth {
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
        /// @brief Path to the engine content root directory
        inline static fs::path sEngineContentRoot;

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
}  // namespace Nth