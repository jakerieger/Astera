/*
 *  Filename: TextureLoader.hpp
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

#include "AssetManager.hpp"
#include "EngineCommon.hpp"
#include "ResourceManager.hpp"
#include "Texture.hpp"
#include "Rendering/GLUtils.hpp"

#include <stb_image.h>

namespace Astera {
    class TextureLoaderSprite final : public ResourceLoader<TextureSprite> {
        TextureSprite LoadImpl(RenderContext& context, ArenaAllocator& allocator, const u64 id) override {
            u32 texId;
            GLCall(glGenTextures, 1, &texId);

            // load image file bytes
            auto imageBytes = AssetManager::GetAssetData(id);
            if (!imageBytes.has_value()) {
                throw std::runtime_error("Texture asset ID not found, failed to get bytes");
            }

            i32 w, h, channels;
            stbi_set_flip_vertically_on_load(true);  // For OpenGL
            auto* data = stbi_load_from_memory(&*imageBytes->data(), (i32)imageBytes->size(), &w, &h, &channels, 0);
            if (!data) {
                throw std::runtime_error("Failed to load image data");
            }

            GLenum format = GL_RGBA;
            if (channels == 1)
                format = GL_RED;
            else if (channels == 3)
                format = GL_RGB;
            else if (channels == 4)
                format = GL_RGBA;

            GLCall(glBindTexture, GL_TEXTURE_2D, texId);
            GLCall(glTexImage2D, GL_TEXTURE_2D, 0, CAST<int>(format), w, h, 0, format, GL_UNSIGNED_BYTE, data);
            GLCall(glGenerateMipmap, GL_TEXTURE_2D);

            GLCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
            GLCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
            GLCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            GLCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            stbi_image_free(data);
            return TextureSprite(texId, w, h, channels);
        }
    };
}  // namespace Astera