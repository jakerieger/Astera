/*
 *  Filename: TextureManager.cpp
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

#include "TextureManager.hpp"
#include "Log.hpp"
#include "Rendering/GLUtils.hpp"

#include <Vendor/stb_image.h>

namespace Astera {
    shared_ptr<TextureManager> TextureManager::sManager;
    unordered_map<string, TextureID> TextureManager::sCache;

    void TextureManager::Initialize() {
        sManager = make_shared<TextureManager>();
        Log::Debug("TextureManager", "Initialized TextureManager");
    }

    void TextureManager::Shutdown() {
        if (!sCache.empty()) {
            for (const auto& id : sCache | std::views::values) {
                Log::Debug("TextureManager", "Unloading texture id `{}`", id);
                GLCall(glDeleteTextures, 1, &id);
            }
        }
        sManager.reset();
    }

    TextureID TextureManager::GetTextureByName(const string& name) {
        return sCache.find(name)->second;
    }

    TextureID TextureManager::Load(const fs::path& filename) {
        u32 id;
        glGenTextures(1, &id);

        int w, h, channels;
        stbi_set_flip_vertically_on_load(true);  // For OpenGL
        const auto data = stbi_load(filename.string().c_str(), &w, &h, &channels, 0);
        if (!data) { Log::Error("TextureManager", "Failed to load image: `{}`", filename.string()); }

        GLenum format = GL_RGBA;
        if (channels == 1) format = GL_RED;
        else if (channels == 3) format = GL_RGB;
        else if (channels == 4) format = GL_RGBA;

        GLCall(glBindTexture, GL_TEXTURE_2D, id);
        GLCall(glTexImage2D, GL_TEXTURE_2D, 0, CAST<int>(format), w, h, 0, format, GL_UNSIGNED_BYTE, data);
        GLCall(glGenerateMipmap, GL_TEXTURE_2D);

        GLCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        GLCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        GLCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        GLCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        stbi_image_free(data);

        sCache[filename.string()] = id;
        Log::Debug("TextureManager", "Loaded texture `{}` with OpenGL id `{}`", filename.string().c_str(), id);

        return id;
    }

    shared_ptr<TextureManager> TextureManager::GetManager() {
        if (!sManager) Initialize();
        return sManager;
    }
}  // namespace Astera