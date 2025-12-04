/// @author Jake Rieger
/// @created 11/29/25
///
#include "ShaderManager.hpp"
#include "Content.hpp"
#include "Log.hpp"

namespace Nth {
    unordered_map<string, shared_ptr<Shader>> ShaderManager::sCache;
    shared_ptr<ShaderManager> ShaderManager::sManager;

    void ShaderManager::Initialize() {
        sManager = make_shared<ShaderManager>();

        // Load internal shaders
        const auto vertShader          = Content::Get<ContentType::Shader, true>("Sprite.vert");
        const auto fragShader          = Content::Get<ContentType::Shader, true>("Sprite.frag");
        sCache[Shaders::Sprite.data()] = Shader::FromFile(vertShader, fragShader);

        Log::Debug("ShaderManager", "Loaded engine shaders");
    }

    void ShaderManager::Shutdown() {
        for (auto [name, shader] : sCache) {
            shader.reset();
            Log::Debug("ShaderManager", "Destroyed shader `{}`", name);
        }
        sCache.clear();
        sManager.reset();
    }

    shared_ptr<Shader> ShaderManager::GetShader(std::string_view name) {
        const auto iter = sCache.find(name.data());
        if (iter != sCache.end()) {
            return iter->second;
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