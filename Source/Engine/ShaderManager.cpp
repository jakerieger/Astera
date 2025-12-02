/// @author Jake Rieger
/// @created 11/29/25
///
#include "ShaderManager.hpp"
#include "Content.hpp"
#include "Log.hpp"

namespace Nth {
    unordered_map<string, Shader> ShaderManager::sCache;
    shared_ptr<ShaderManager> ShaderManager::sManager;

    void ShaderManager::Initialize() {
        sManager = make_shared<ShaderManager>();

        // Load internal shaders
        Shader spriteShader;
        spriteShader.FromFile(Content::GetContentPath("Shaders/Sprite.vert"),
                              Content::GetContentPath("Shaders/Sprite.frag"));
        sCache.insert_or_assign(Shaders::Sprite.data(), std::move(spriteShader));

        Log::Debug("ShaderManager", "Loaded engine shaders");
    }

    void ShaderManager::Shutdown() {
        for (auto [name, shader] : sCache) {
            shader.Destroy();
            Log::Debug("ShaderManager", "Destroyed shader `{}`", name);
        }
        sCache.clear();
        sManager.reset();
    }

    Shader* ShaderManager::GetShader(std::string_view name) {
        const auto iter = sCache.find(name.data());
        if (iter != sCache.end()) {
            return &iter->second;
        } else {
            Log::Error("ShaderManager", "Could not find shader `{}` in cache", name);
            return nullptr;
        }
    }

    shared_ptr<ShaderManager> ShaderManager::GetManager() {
        if (!sManager) Initialize();
        return sManager;
    }
}  // namespace Nth