/// @author Jake Rieger
/// @created 11/30/25
///

#pragma once

#include "CommonPCH.hpp"

namespace Nth {
    enum class ContentType {
        Audio,
        Scene,
        Script,
        Shader,
        Sprite,
    };

    template<ContentType type, bool engine = false>
    class Content {
        static constexpr std::string_view kContentRoot       = "Content";
        static constexpr std::string_view kEngineContentRoot = "EngineContent";

        static constexpr std::string_view kSceneRoot  = "Scenes";
        static constexpr std::string_view kScriptRoot = "Scripts";
        static constexpr std::string_view kShaderRoot = "Shaders";
        static constexpr std::string_view kSpriteRoot = "Sprites";
        static constexpr std::string_view kAudioRoot  = "Audio";

    public:
        inline static fs::path Get(const fs::path& filename) {
            fs::path contentPath = fs::current_path() / kContentRoot;
            if constexpr (engine) { contentPath /= kEngineContentRoot; }

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
    };
}  // namespace Nth
