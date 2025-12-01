// Author: Jake Rieger
// Created: 11/27/25.
//

#include <Game.hpp>
#include <InputCodes.hpp>
#include <ShaderManager.hpp>
#include <TextureManager.hpp>
#include <Rendering/Shader.hpp>
#include <Content.hpp>
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
            const auto spriteTex = TextureManager::Load(Content::GetContentPath("Sprites/ball.png"));

            auto& state       = GetActiveScene()->GetState();
            const auto entity = state.CreateEntity();
            auto& sprite      = state.AddComponent<SpriteRenderer>(entity);
            sprite.textureId  = spriteTex;

            auto& transform = state.GetComponent<Transform>(entity);
            transform.scale = {64, 64};

            auto* spriteShader = ShaderManager::GetShader(Shaders::Sprite);
            N_ASSERT(spriteShader);  // Quick check that the shader actually loaded
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