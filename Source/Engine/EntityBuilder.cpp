/*
 *  Filename: EntityBuilder.cpp
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

#include "EntityBuilder.hpp"

#include "AssetManager.hpp"
#include "Content.hpp"
#include "Log.hpp"
#include "ScriptEngine.hpp"
#include "Sound.hpp"
#include "Components/SoundSource.hpp"

namespace Astera {
    EntityBuilder& EntityBuilder::SetTransform(const TransformDescriptor& descriptor) {
        auto& state        = mScene->GetState();
        auto& transform    = state.GetTransform(mEntity);
        transform.position = descriptor.position;
        transform.rotation = descriptor.rotation;
        transform.scale    = descriptor.scale;
        return *this;
    }

    EntityBuilder& EntityBuilder::AddBehavior(const BehaviorDescriptor& descriptor, ScriptEngine& scriptEngine) {
        // Load script
        const auto scriptSource = AssetManager::GetAssetText(descriptor.script);
        if (!scriptSource.has_value()) {
            throw std::runtime_error("Could not load script data");
        }
        scriptEngine.LoadScript(*scriptSource, descriptor.script);

        // TODO: Compile to, load from, and cache Lua byyecode

        // Update entity
        auto& state     = mScene->GetState();
        auto& behavior  = state.AddComponent<Behavior>(mEntity);
        behavior.script = descriptor.script;

        return *this;
    }

    EntityBuilder& EntityBuilder::AddSpriteRenderer(const SpriteRendererDescriptor& descriptor) {
        // Load texture
        auto& resourceManager = mScene->GetResourceManager();
        const auto loadResult = resourceManager.LoadResource<TextureSprite>(descriptor.texture);
        if (!loadResult) {
            throw std::runtime_error("Could not load texture sprite");
        }
        const ResourceHandle<TextureSprite> spriteHandle =
          resourceManager.FetchResource<TextureSprite>(descriptor.texture);

        if (!spriteHandle->IsValid()) {
            throw std::runtime_error("Could not load texture sprite - handle invalid");
        }

        // Update entity
        auto& sprite    = mScene->GetState().AddComponent<SpriteRenderer>(mEntity);
        sprite.geometry = Geometry::CreateQuad();
        sprite.sprite   = spriteHandle;

        return *this;
    }

    EntityBuilder& EntityBuilder::AddRigidbody2D(const Rigidbody2DDescriptor& descriptor) {
        return *this;
    }

    EntityBuilder& EntityBuilder::AddCollider2D(const Collider2DDescriptor& descriptor) {
        return *this;
    }

    EntityBuilder& EntityBuilder::AddCamera(const CameraDescriptor& descriptor) {
        return *this;
    }

    EntityBuilder& EntityBuilder::AddSoundSource(const SoundSourceDescriptor& descriptor) {
        auto& resMgr      = mScene->GetResourceManager();
        const auto result = resMgr.LoadResource<Sound>(descriptor.sound);
        if (!result) {
            throw std::runtime_error("Could not load sound");
        }

        const ResourceHandle<Sound> soundHandle = resMgr.FetchResource<Sound>(descriptor.sound);
        if (!soundHandle.IsValid()) {
            throw std::runtime_error("Could not load sound. Resource handle invalid.");
        }

        auto& sound  = mScene->GetState().AddComponent<SoundSource>(mEntity);
        sound.sound  = soundHandle;
        sound.volume = descriptor.volume;

        return *this;
    }
}  // namespace Astera