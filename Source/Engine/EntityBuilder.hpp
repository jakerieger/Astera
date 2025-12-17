/*
 *  Filename: EntityBuilder.hpp
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
#include "SceneDescriptor.hpp"
#include "Scene.hpp"

namespace Astera {
    class EntityBuilder {
        Scene* mScene;
        Entity mEntity;

        explicit EntityBuilder(Scene* state) : mScene(state), mEntity() {}

    public:
        ASTERA_CLASS_PREVENT_COPIES(EntityBuilder)

        EntityBuilder(EntityBuilder&& other) noexcept
            : mScene(std::exchange(other.mScene, nullptr)), mEntity(std::exchange(other.mEntity, {})) {};

        EntityBuilder& operator=(EntityBuilder&& other) noexcept {
            if (this != &other) {
                mScene  = std::exchange(other.mScene, nullptr);
                mEntity = std::exchange(other.mEntity, {});
            }
            return *this;
        }

        static EntityBuilder Create(Scene* scene, const string& name) {
            EntityBuilder builder(scene);
            auto& state     = scene->GetState();
            builder.mEntity = state.CreateEntity(name);
            return std::move(builder);
        }

        ASTERA_KEEP Entity Build() const {
            return mEntity;
        }

        EntityBuilder& SetTransform(const TransformDescriptor& descriptor);
        EntityBuilder& AddBehavior(const BehaviorDescriptor& descriptor, class ScriptEngine& scriptEngine);
        EntityBuilder& AddSpriteRenderer(const SpriteRendererDescriptor& descriptor);
        EntityBuilder& AddRigidbody2D(const Rigidbody2DDescriptor& descriptor);
        EntityBuilder& AddCollider2D(const Collider2DDescriptor& descriptor);
        EntityBuilder& AddCamera(const CameraDescriptor& descriptor);
        EntityBuilder& AddSoundSource(const SoundSourceDescriptor& descriptor);
    };
}  // namespace Astera
