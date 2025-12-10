/// @author Jake Rieger
/// @created 11/27/25
///

#include <NthEngine.hpp>

using namespace Nth;

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
        SetWindowIcon("Res/icon.png");

        GetActiveScene()->Load(Content::Get<ContentType::Scene>("Sandbox.xml"), GetScriptEngine());
        const SoundID bgMusic = GetAudioEngine().LoadSound(Content::Get<ContentType::Audio>("bg.wav"));
        GetAudioEngine().SetSoundVolume(bgMusic, .5f);
        GetAudioEngine().PlaySound(bgMusic, true);

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
};

N_ENTRYPOINT {
    Content::SetRuntimeDefaults();
    SandboxGame().Run();
}