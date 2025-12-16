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
#include "SceneState.hpp"

namespace Astera {
    class EntityBuilder {
        SceneState* mState;
        Entity mEntity;

        explicit EntityBuilder(SceneState* state) : mState(state), mEntity() {}

    public:
        ASTERA_CLASS_PREVENT_COPIES(EntityBuilder)

        EntityBuilder(EntityBuilder&& other) noexcept
            : mState(std::exchange(other.mState, nullptr)), mEntity(std::move(other.mEntity)) {};

        EntityBuilder& operator=(EntityBuilder&& other) noexcept {
            if (this != &other) {
                mState  = std::exchange(other.mState, nullptr);
                mEntity = std::move(other.mEntity);
            }
            return *this;
        }

        static EntityBuilder Create(SceneState* state, const string& name) {
            EntityBuilder builder(state);
            builder.mEntity = state->CreateEntity(name);
            return std::move(builder);
        }

        ASTERA_KEEP Entity Build() const {
            return mEntity;
        }

        EntityBuilder& SetTransform(const Vec2& pos, const Vec2& rot, const Vec2& scale);
        EntityBuilder& AddBehavior(u64 id, const string& script);
        EntityBuilder& AddSpriteRenderer(u32 textureId, const GeometryHandle& geometry);
        EntityBuilder& AddRigidbody2D();
        EntityBuilder& AddCollider2D();
        EntityBuilder& AddCamera();
    };
}  // namespace Astera
