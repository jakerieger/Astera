/// @author Jake Rieger
/// @created 12/3/2025
///

#pragma once

#include "CommonPCH.hpp"

namespace sol {
    class state;
}

namespace Nth {
    class Game;
}

namespace Nth::Input {
    class InputManager {
    public:
        N_CLASS_PREVENT_MOVES_COPIES(InputManager)

        InputManager() = default;

        // Getters
        bool GetKeyDown(i32 key);
        bool GetKeyUp(i32 key);
        bool GetMouseButtonDown(i32 button);
        bool GetMouseButtonUp(i32 button);

        N_ND i32 GetMouseX() const;
        N_ND i32 GetMouseY() const;
        N_ND f32 GetMouseDeltaX() const;
        N_ND f32 GetMouseDeltaY() const;

        void ResetMouseDeltas();

    private:
        friend Nth::Game;

        void RegisterLuaGlobals(sol::state& state);

        // Private update methods
        void UpdateKeyState(u16 key, bool pressed);
        void UpdateMouseButtonState(u16 button, bool pressed);
        void UpdateMousePosition(f64 x, f64 y);
        void SetEnabled(bool enabled);

        struct KeyState {
            bool pressed  = false;
            bool released = false;
        };

        unordered_map<i32, KeyState> mKeyStates;
        unordered_map<i32, KeyState> mMouseStates;
        i32 mMouseX = 0, mMouseY = 0;
        f32 mMouseDeltaX = 0.f, mMouseDeltaY = 0.f;
        bool mEnabled = true;
    };
}  // namespace Nth::Input