#include "AssetManager.hpp"
#include "EntityBuilder.hpp"

#include <AsteraCore.hpp>

using namespace Astera;

class SpaceInvadersDemo final : public Game {
public:
    SpaceInvadersDemo() : Game("Space Invaders [Demo]", 1280, 720) {
        AssetManager::SetWorkingDirectory(fs::current_path() / "Content");
    }

    void LoadContent() override {
        auto* scene = GetActiveScene();
        scene->Load(Content::Get<ContentType::Scene>("Main.xml"), GetScriptEngine());
    }

    void OnKeyDown(const u32 keyCode) override {
        using namespace Input;
        if (keyCode == Keys::Escape) {
            Quit();
        }
    }
};

ASTERA_RUN_GAME(SpaceInvadersDemo)
