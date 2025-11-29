// Author: Jake Rieger
// Created: 11/27/25.
//

#include "ShaderManager.hpp"
#include "TextureManager.hpp"
#include "Rendering/Shader.hpp"

#include <Game.hpp>
#include <InputCodes.hpp>
#include <Log.hpp>

namespace Nth {
    class SandboxGame final : public Game {
    public:
        SandboxGame() : Game("Sandbox", 1280, 720) {}

        void OnKeyDown(u32 keyCode) override {
            if (keyCode == Input::Keys::Escape) { Quit(); }

            if (keyCode == Input::Keys::F11) { ToggleFullscreen(); }
        }

        void OnAwake() override {
            const auto spriteTex = TextureManager::Load("/home/jr/Code/NthEngine/Content/Sprites/ball.png");

            auto& state       = GetActiveScene()->GetState();
            const auto entity = state.CreateEntity();
            auto& sprite      = state.AddComponent<SpriteRenderer>(entity);
            sprite.textureId  = spriteTex;

            auto* spriteShader = ShaderManager::GetShader(Shaders::Sprite);
            N_ASSERT(spriteShader);
        }

        void OnUpdate(const Clock& clock) override {}

        void OnLateUpdate() override {}

        void OnDestroyed() override {}
    };
}  // namespace Nth

int main() {
    Nth::SandboxGame game;
    game.Run();
}