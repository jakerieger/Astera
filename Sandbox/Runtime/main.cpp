/// @author Jake Rieger
/// @created 11/27/25
///

#include <Game.hpp>
#include <InputCodes.hpp>
#include <Content.hpp>
#include <AudioEngine.hpp>

namespace Nth {
    class SandboxGame final : public Game {
    public:
        SandboxGame() : Game("Sandbox", 1280, 720) {}

        void OnKeyDown(u32 keyCode) override {
            Game::OnKeyDown(keyCode);
            using namespace Input;
            switch (keyCode) {
                case Keys::Escape:
                    Quit();
                    break;
                case Keys::F11:
                    ToggleFullscreen();
                    break;
                case Keys::F10: {
                    auto& debug = GetDebugManager();
                    debug.SetOverlayEnabled("PhysicsDebugLayer", !debug.GetOverlayEnabled("PhysicsDebugLayer"));
                } break;
                default:
                    return;
            }
        }

        void OnAwake() override {
            GetActiveScene()->Load(Content::Get<ContentType::Scene>("Sandbox.xml"), GetScriptEngine());

            // Audio engine test
            mTestSound = GetAudioEngine().LoadSound(Content::Get<ContentType::Audio>("bg.wav"));
            // GetAudioEngine().SetSoundVolume(mTestSound, .5f);
            // GetAudioEngine().PlaySound(mTestSound, true);

            Game::OnAwake();
        }

        void OnUpdate(const Clock& clock) override {
            Game::OnUpdate(clock);
        }

        void OnLateUpdate() override {
            Game::OnLateUpdate();
        }

        void OnDestroyed() override {
            Game::OnDestroyed();
        }

        void OnResize(u32 width, u32 height) override {
            Game::OnResize(width, height);
        }

    private:
        SoundID mTestSound {kInvalidSoundID};
    };
}  // namespace Nth

N_ENTRYPOINT {
    Nth::Content::SetRuntimeDefaults();
    Nth::SandboxGame game;
    game.Run();
}