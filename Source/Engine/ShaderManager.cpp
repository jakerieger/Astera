/*
 *  Filename: ShaderManager.cpp
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

#include "ShaderManager.hpp"
#include "Content.hpp"
#include "Log.hpp"

namespace Nth {
    unordered_map<string, shared_ptr<Shader>> ShaderManager::sCache;
    shared_ptr<ShaderManager> ShaderManager::sManager;

    void ShaderManager::Initialize() {
        sManager = make_shared<ShaderManager>();

        // Load internal shaders
        const auto spriteVert          = Content::Get<ContentType::Shader, true>("Sprite.vert");
        const auto spriteFrag          = Content::Get<ContentType::Shader, true>("Sprite.frag");
        sCache[Shaders::Sprite.data()] = Shader::FromFile(spriteVert, spriteFrag);

        const auto spriteInstVert               = Content::Get<ContentType::Shader, true>("SpriteInstanced.vert");
        const auto spriteInstFrag               = Content::Get<ContentType::Shader, true>("SpriteInstanced.frag");
        sCache[Shaders::SpriteInstanced.data()] = Shader::FromFile(spriteInstVert, spriteInstFrag);

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