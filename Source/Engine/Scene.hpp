// Author: Jake Rieger
// Created: 11/28/25.
//

#pragma once

#include "Clock.hpp"
#include "CommonPCH.hpp"
#include "SceneState.hpp"
#include "Rendering/RenderContext.hpp"

namespace Nth {
    class Scene {
    public:
        Scene()  = default;
        ~Scene() = default;

        N_CLASS_PREVENT_MOVES_COPIES(Scene)

        void Awake() {
            N_UNUSED(this);
        }

        void Update(const Clock& clock) {
            N_UNUSED(clock);
        }

        void LateUpdate() {
            N_UNUSED(this);
        }

        void Destroyed() {
            N_UNUSED(this);
        }

        void Render(RenderContext& context) {
            const auto iter = mState.View<Transform, SpriteRenderer>().each();
            for (auto [entity, transform, sprite] : iter) {
                context.Submit(DrawSpriteCommand {sprite.textureId, transform, {1, 1, 1, 1}});
            }
        }

        N_ND SceneState& GetState() {
            return mState;
        }

    private:
        SceneState mState;
    };
}  // namespace Nth
