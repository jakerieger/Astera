/*
 *  Filename: Scene.cpp
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

#include "Scene.hpp"
#include "SceneParser.hpp"
#include "ScriptTypeRegistry.hpp"
#include "Log.hpp"

namespace Nth {
    Scene::~Scene() {
        mState.Reset();
    }

    void Scene::Awake(ScriptEngine& scriptEngine) {
        const auto iter = mState.View<Transform, Behavior>().each();
        for (auto [entity, transform, behavior] : iter) {
            BehaviorEntity behaviorEntity(mState.GetEntityName(entity), &transform);
            scriptEngine.CallAwakeBehavior(behavior.id, behaviorEntity);
        }
    }

    void Scene::Update(ScriptEngine& scriptEngine, const Clock& clock) {
        const auto iter = mState.View<Transform, Behavior>().each();
        for (auto [entity, transform, behavior] : iter) {
            BehaviorEntity behaviorEntity(mState.GetEntityName(entity), &transform);
            scriptEngine.CallUpdateBehavior(behavior.id, behaviorEntity, clock);
        }
    }

    void Scene::LateUpdate(ScriptEngine& scriptEngine) {
        const auto iter = mState.View<Transform, Behavior>().each();
        for (auto [entity, transform, behavior] : iter) {
            BehaviorEntity behaviorEntity(mState.GetEntityName(entity), &transform);
            scriptEngine.CallLateUpdateBehavior(behavior.id, behaviorEntity);
        }
    }

    void Scene::Destroyed(ScriptEngine& scriptEngine) {
        const auto iter = mState.View<Transform, Behavior>().each();
        for (auto [entity, transform, behavior] : iter) {
            BehaviorEntity behaviorEntity(mState.GetEntityName(entity), &transform);
            scriptEngine.CallDestroyedBehavior(behavior.id, behaviorEntity);
        }
    }

    void Scene::Render(RenderContext& context) {
        u32 screenWidth = 0, screenHeight = 0;
        context.GetViewportDimensions(screenWidth, screenHeight);

        const auto iter = mState.View<Transform, SpriteRenderer>().each();
        for (auto [entity, transform, sprite] : iter) {
            context.Submit(DrawSpriteCommand {sprite, transform, {screenWidth, screenHeight}, {1, 1, 1, 1}});
        }
    }

    void Scene::Load(const fs::path& filename, ScriptEngine& scriptEngine) {
        mState.Reset();
        SceneDescriptor descriptor;
        SceneParser::DeserializeDescriptor(filename, descriptor);
        SceneParser::DescriptorToState(descriptor, mState, scriptEngine);

        Log::Debug("Scene", "Loaded scene: `{}`", descriptor.name);
    }

    void Scene::Load(const string& source, ScriptEngine& scriptEngine) {
        mState.Reset();
        SceneDescriptor descriptor;
        SceneParser::DeserializeDescriptor(source, descriptor);
        SceneParser::DescriptorToState(descriptor, mState, scriptEngine);

        Log::Debug("Scene", "Loaded scene: `{}`", descriptor.name);
    }
}  // namespace Nth