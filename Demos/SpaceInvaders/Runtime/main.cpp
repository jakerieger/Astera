#include <AsteraCore.hpp>

using namespace Astera;

class SpaceInvadersDemo final : public Game {
public:
    SpaceInvadersDemo() : Game("Space Invaders [Astera Demo]", 1280, 720) {}

    void OnAwake() override {
        auto& debug = GetDebugManager();
        debug.SetOverlayEnabled("ImGuiDebugLayer", false);
        debug.SetOverlayEnabled("PhysicsDebugLayer", false);
        Game::OnAwake();
    }
};

ASTERA_ENTRYPOINT {
    Content::SetRuntimeDefaults();
    SpaceInvadersDemo().Run();
}