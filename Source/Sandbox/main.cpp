// Author: Jake Rieger
// Created: 11/27/25.
//

#include <Game.hpp>
#include <InputCodes.hpp>
#include <ShaderManager.hpp>
#include <TextureManager.hpp>
#include <Rendering/Shader.hpp>
#include <Content.hpp>
#include <Rendering/Geometry.hpp>
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
            N_ASSERT(ShaderManager::GetShader(Shaders::Sprite));  // Quick check that the shader actually loaded
            mQuad = Geometry::CreateQuad(1.f, 1.f);

            const auto spriteTex = TextureManager::Load(Content::GetContentPath("Sprites/ball.png"));

            auto& state       = GetActiveScene()->GetState();
            const auto entity = state.CreateEntity();
            auto& sprite      = state.AddComponent<SpriteRenderer>(entity);
            sprite.textureId  = spriteTex;
            sprite.geometry   = mQuad.get();  // I hate htis but we'll try it for now

            auto& transform    = state.GetComponent<Transform>(entity);
            transform.position = {640, 360};  // Center of screen
            transform.scale    = {64, 64};
        }

        void OnUpdate(const Clock& clock) override {}

        void OnLateUpdate() override {}

        void OnDestroyed() override {
            mQuad->Destroy();
        }

    private:
        shared_ptr<Geometry> mQuad;
    };
}  // namespace Nth

int main() {
    Nth::SandboxGame game;
    game.Run();
}