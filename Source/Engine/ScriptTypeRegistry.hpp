/*
 *  Filename: ScriptTypeRegistry.hpp
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

#include "Scene.hpp"
#include "ScriptEngine.hpp"

namespace Astera {
    struct BehaviorEntity {
        u32 id;
        string name;
        Transform* transform;

        explicit BehaviorEntity(u32 id, const string& name, Transform* transform)
            : id(id), name(name), transform(transform) {}
    };

    template<>
    struct LuaTypeTraits<BehaviorEntity> {
        static constexpr std::string_view typeName = "Entity";

        static void RegisterMembers(sol::usertype<BehaviorEntity>& usertype) {
            usertype["name"]      = &BehaviorEntity::name;
            usertype["transform"] = &BehaviorEntity::transform;
        }
    };

    template<>
    struct LuaTypeTraits<Vec2> {
        static constexpr std::string_view typeName = "Vec2";

        static void RegisterMembers(sol::usertype<Vec2>& usertype) {
            usertype[sol::call_constructor] = sol::constructors<Vec2(), Vec2(f32, f32)>();
            usertype["x"]                   = &Vec2::x;
            usertype["y"]                   = &Vec2::y;
        }
    };

    template<>
    struct LuaTypeTraits<Clock> {
        static constexpr std::string_view typeName = "Clock";

        static void RegisterMembers(sol::usertype<Clock>& usertype) {
            usertype["GetDeltaTimePrecise"] = &Clock::GetDeltaTimePrecise;
            usertype["GetDeltaTime"]        = &Clock::GetDeltaTime;
            usertype["GetTotalTime"]        = &Clock::GetTotalTime;
            usertype["GetFramesPerSecond"]  = &Clock::GetFramesPerSecond;
            usertype["GetRawCounter"]       = &Clock::GetRawCounter;
            usertype["GetCounterFrequency"] = &Clock::GetCounterFrequency;
        }
    };

    template<>
    struct LuaTypeTraits<Transform> {
        static constexpr std::string_view typeName = "Transform";

        static void RegisterMembers(sol::usertype<Transform>& usertype) {
            usertype["position"]  = &Transform::position;
            usertype["rotation"]  = &Transform::rotation;
            usertype["scale"]     = &Transform::scale;
            usertype["Translate"] = &Transform::Translate;
            usertype["Rotate"]    = &Transform::Rotate;
            usertype["Scale"]     = &Transform::Scale;
        }
    };

    template<>
    struct LuaTypeTraits<SceneState> {
        static constexpr std::string_view typeName = "SceneState";

        static void RegisterMembers(sol::usertype<SceneState>& usertype) {
            usertype["FindEntityByName"] = [](SceneState& scene, const string& name) -> sol::optional<Entity> {
                const auto view = scene.View<Transform>();
                for (auto entity : view) {
                    if (scene.GetEntityName(entity) == name) { return entity; }
                }
                return sol::nullopt;
            };

            usertype["GetEntityTransform"] = [](SceneState& scene, Entity entity) -> Transform* {
                auto& transform = scene.GetTransform(entity);
                return &transform;
            };
        }
    };

    class ScriptTypeRegistry {
    public:
        inline static void RegisterTypes(ScriptEngine& engine) {
            engine.RegisterTypes<BehaviorEntity, Clock, Transform, SceneState, Vec2>();
        }
    };
}  // namespace Astera