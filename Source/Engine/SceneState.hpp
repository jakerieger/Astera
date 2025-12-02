/// @author Jake Rieger
/// @created 11/28/25
///

#pragma once

#include "CommonPCH.hpp"
#include "Log.hpp"

#pragma region Components
#include "Components/Camera.hpp"
#include "Components/Transform.hpp"
#include "Components/SpriteRenderer.hpp"
#pragma endregion

#include <entt/entt.hpp>

namespace Nth {
    using Entity = entt::entity;

    template<typename T>
    concept ValidComponent =
      std::is_same_v<T, Transform> || std::is_same_v<T, SpriteRenderer> || std::is_same_v<T, Camera>;

    class SceneState {
    public:
        SceneState() {
            mRegistry.on_construct<Transform>().connect<[](entt::registry& reg, entt::entity entity) {
                Log::Debug("SceneState", "Entity created `{}`", (u32)entity);
            }>();

            mRegistry.on_destroy<Transform>().connect<[](entt::registry& reg, entt::entity entity) {
                Log::Debug("SceneState", "Entity destroyed `{}`", (u32)entity);
            }>();
        }

        ~SceneState() = default;

        /// @brief Creates a new entity in the scene tree. All entities are required to have a Transform component
        /// and this method automatically adds it when creating the new entity.
        Entity CreateEntity() {
            const auto entity = mRegistry.create();
            mRegistry.emplace<Transform>(entity);
            return entity;
        }

        void DestroyEntity(Entity entity) {
            mRegistry.destroy(entity);
        }

        template<typename Component, typename... Args>
            requires ValidComponent<Component>
        Component& AddComponent(Entity entity, Args&&... args) {
            mRegistry.emplace<Component>(entity, std::forward<Args>(args)...);
            return mRegistry.get<Component>(entity);
        }

        template<typename Component>
            requires ValidComponent<Component>
        Component& GetComponent(Entity entity) {
            return mRegistry.get<Component>(entity);
        }

        template<typename... Components>
            requires(ValidComponent<Components> && ...)
        auto View() {
            return mRegistry.view<Components...>();
        }

        template<typename Component>
            requires ValidComponent<Component>
        vector<Entity> GetAllEntitiesWithComponent() {
            vector<Entity> entities;

            const auto iter = mRegistry.view<Component>().each();
            for (const auto [entity, component] : iter) {
                entities.push_back(entity);
            }

            return entities;
        }

        N_ND size_t GetEntityCount() const {
            const auto iter = mRegistry.view<Transform>().each();
            return CAST<size_t>(Nth::Distance(iter));
        }

    private:
        entt::registry mRegistry {};
    };
}  // namespace Nth
