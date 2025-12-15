#include "AssetManager.hpp"

#include <AsteraCore.hpp>

using namespace Astera;

class SpaceInvadersDemo final : public Game {
public:
    SpaceInvadersDemo() : Game("Space Invaders [Demo]", 1280, 720) {
        AssetManager::SetWorkingDirectory(fs::current_path() / "Content");
    }

    void LoadContent() override {
        GetActiveScene()->Load(Content::Get<ContentType::Scene>("Main.xml"), GetScriptEngine());
    }

    void OnKeyDown(const u32 keyCode) override {
        using namespace Input;
        if (keyCode == Keys::Escape) {
            Quit();
        }
    }

    void OnKey(u32 keyCode) override {
        using namespace Input;
        if (keyCode == Keys::Space) {
            auto nameSuffix = Math::RandomInt(0, std::numeric_limits<i32>::max());
            const auto name = fmt::format("Entity_{}", nameSuffix);
            // spawn new entity
            GetActiveScene()->GetState().CreateEntity(name);
        }
    }
};

ASTERA_RUN_GAME(SpaceInvadersDemo)
