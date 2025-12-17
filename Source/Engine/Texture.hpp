/*
 *  Filename: Texture.hpp
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

#include "EngineCommon.hpp"
#include "Rendering/GLUtils.hpp"

namespace Astera {
    class TextureSprite;
    class TextureSpriteSheet;
    class TextureAnimation;
    class TextureUI;

    template<typename T>
    concept IsTexture = std::is_same_v<T, TextureSprite> || std::is_same_v<T, TextureSpriteSheet> ||
                        std::is_same_v<T, TextureAnimation> || std::is_same_v<T, TextureUI>;

    inline constexpr GLuint kInvalidTextureID {0};

    class TextureSprite {
        friend class TextureLoaderSprite;

    public:
        void Bind(u32 slot = 0) const {
            ASTERA_ASSERT_MSG(slot <= 32, "Slot number too large. Slot must be <= 32");

            if (mId != kInvalidTextureID) {
                GLCall(glActiveTexture, GL_TEXTURE0 + slot);
                GLCall(glBindTexture, GL_TEXTURE_2D, mId);
            }
        }

        void Unbind() const {
            if (mId != kInvalidTextureID) {
                GLCall(glBindTexture, GL_TEXTURE_2D, 0);
            }
        }

        TextureSprite(TextureSprite&& other) noexcept : mId(std::exchange(other.mId, kInvalidTextureID)) {}

        TextureSprite& operator=(TextureSprite&& other) noexcept {
            if (this != &other) {
                mId = std::exchange(other.mId, kInvalidTextureID);
            }
            return *this;
        }

        ASTERA_CLASS_PREVENT_COPIES(TextureSprite)

        ~TextureSprite() {
            GLCall(glDeleteTextures, 1, &mId);
        }

        bool IsValid() const {
            return mId != kInvalidTextureID;
        }

        GLuint GetID() const {
            return mId;
        }

        i32 GetWidth() const {
            return mWidth;
        }

        i32 GetHeight() const {
            return mHeight;
        }

        i32 GetChannels() const {
            return mChannels;
        }

    private:
        GLuint mId;
        i32 mWidth, mHeight;
        i32 mChannels;

        explicit TextureSprite(GLuint id, i32 width, i32 height, i32 channels)
            : mId(id), mWidth(width), mHeight(height), mChannels(channels) {}
    };
}  // namespace Astera