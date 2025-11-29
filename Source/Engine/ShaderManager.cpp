// Author: Jake Rieger
// Created: 11/29/25.
//

#include "ShaderManager.hpp"

#include "Log.hpp"

namespace Nth {
    unordered_map<string, Shader> ShaderManager::sCache;
    shared_ptr<ShaderManager> ShaderManager::sManager;

    void ShaderManager::Initialize() {
        sManager = make_shared<ShaderManager>();

        // Load internal shaders
        Shader spriteShader;
        // TODO: Obviously remove hard-coded paths in the future, this is fine for early dev
        spriteShader.FromFile("/home/jr/Code/NthEngine/Source/Shaders/Sprite.vert",
                              "/home/jr/Code/NthEngine/Source/Shaders/Sprite.frag");
        sCache[Shaders::Sprite] = std::move(spriteShader);
    }

    void ShaderManager::Shutdown() {
        for (auto [_, shader] : sCache) {
            shader.Destroy();
        }
        sCache.clear();
        sManager.reset();
    }

    Shader* ShaderManager::GetShader(const string& name) {
        const auto iter = sCache.find(name);
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