/// @author Jake Rieger
/// @created 12/3/2025
///

#include "Input.hpp"
#include "InputCodes.hpp"

#include <sol/sol.hpp>

namespace Nth::Input {
    bool InputManager::GetKeyDown(i32 key) {
        return mKeyStates[key].pressed;
    }

    bool InputManager::GetKeyUp(i32 key) {
        return mKeyStates[key].released;
    }

    bool InputManager::GetMouseButtonDown(i32 button) {
        return mMouseStates[button].pressed;
    }

    bool InputManager::GetMouseButtonUp(i32 button) {
        return mMouseStates[button].released;
    }

    i32 InputManager::GetMouseX() const {
        return mMouseX;
    }

    i32 InputManager::GetMouseY() const {
        return mMouseY;
    }

    f32 InputManager::GetMouseDeltaX() const {
        return mMouseDeltaX;
    }

    f32 InputManager::GetMouseDeltaY() const {
        return mMouseDeltaY;
    }

    void InputManager::ResetMouseDeltas() {
        mMouseDeltaX = 0.0f;
        mMouseDeltaY = 0.0f;
    }

    void InputManager::RegisterLuaGlobals(sol::state& state) {
        state["KEY_A"]         = Keys::A;
        state["KEY_B"]         = Keys::B;
        state["KEY_C"]         = Keys::C;
        state["KEY_D"]         = Keys::D;
        state["KEY_E"]         = Keys::E;
        state["KEY_F"]         = Keys::F;
        state["KEY_G"]         = Keys::G;
        state["KEY_H"]         = Keys::H;
        state["KEY_I"]         = Keys::I;
        state["KEY_J"]         = Keys::J;
        state["KEY_K"]         = Keys::K;
        state["KEY_L"]         = Keys::L;
        state["KEY_M"]         = Keys::M;
        state["KEY_N"]         = Keys::N;
        state["KEY_O"]         = Keys::O;
        state["KEY_P"]         = Keys::P;
        state["KEY_Q"]         = Keys::Q;
        state["KEY_R"]         = Keys::R;
        state["KEY_S"]         = Keys::S;
        state["KEY_T"]         = Keys::T;
        state["KEY_U"]         = Keys::U;
        state["KEY_V"]         = Keys::V;
        state["KEY_W"]         = Keys::W;
        state["KEY_X"]         = Keys::X;
        state["KEY_Y"]         = Keys::Y;
        state["KEY_Z"]         = Keys::Z;
        state["KEY_1"]         = Keys::Num1;
        state["KEY_2"]         = Keys::Num2;
        state["KEY_3"]         = Keys::Num3;
        state["KEY_4"]         = Keys::Num4;
        state["KEY_5"]         = Keys::Num5;
        state["KEY_6"]         = Keys::Num6;
        state["KEY_7"]         = Keys::Num7;
        state["KEY_8"]         = Keys::Num8;
        state["KEY_9"]         = Keys::Num9;
        state["KEY_0"]         = Keys::Num0;
        state["KEY_MINUS"]     = Keys::Minus;
        state["KEY_EQUAL"]     = Keys::Equal;
        state["KEY_BACKSPACE"] = Keys::Backspace;
        state["KEY_TAB"]       = Keys::Tab;
        state["KEY_HOME"]      = Keys::Home;
        state["KEY_LEFT"]      = Keys::Left;
        state["KEY_UP"]        = Keys::Up;
        state["KEY_RIGHT"]     = Keys::Right;
        state["KEY_DOWN"]      = Keys::Down;
        state["KEY_ESCAPE"]    = Keys::Escape;
        state["KEY_ENTER"]     = Keys::Enter;
        state["KEY_SPACE"]     = Keys::Space;
        state["KEY_LCTRL"]     = Keys::LeftControl;
        state["KEY_RCTRL"]     = Keys::RightControl;
        state["KEY_F1"]        = Keys::F1;
        state["KEY_F2"]        = Keys::F2;
        state["KEY_F3"]        = Keys::F3;
        state["KEY_F4"]        = Keys::F4;
        state["KEY_F5"]        = Keys::F5;
        state["KEY_F6"]        = Keys::F6;
        state["KEY_F7"]        = Keys::F7;
        state["KEY_F8"]        = Keys::F8;
        state["KEY_F9"]        = Keys::F9;
        state["KEY_F10"]       = Keys::F10;
        state["KEY_F11"]       = Keys::F11;
        state["KEY_F12"]       = Keys::F12;
        state["MOUSE_LEFT"]    = MouseButtons::Left;
        state["MOUSE_RIGHT"]   = MouseButtons::Right;
        state["MOUSE_MIDDLE"]  = MouseButtons::Middle;

        state.new_usertype<InputManager>("InputManager",
                                         "GetKeyDown",
                                         &InputManager::GetKeyDown,
                                         "GetKeyUp",
                                         &InputManager::GetKeyUp,
                                         "GetMouseButtonDown",
                                         &InputManager::GetMouseButtonDown,
                                         "GetMouseButtonUp",
                                         &InputManager::GetMouseButtonUp,
                                         "GetMouseX",
                                         &InputManager::GetMouseX,
                                         "GetMouseY",
                                         &InputManager::GetMouseY,
                                         "GetMouseDeltaX",
                                         &InputManager::GetMouseDeltaX,
                                         "GetMouseDeltaY",
                                         &InputManager::GetMouseDeltaY);
        state["Input"] = this;
    }

    void InputManager::UpdateKeyState(const u16 key, const bool pressed) {
        if (!mEnabled) return;

        mKeyStates[key].pressed  = pressed;
        mKeyStates[key].released = !pressed;
    }

    void InputManager::UpdateMouseButtonState(const u16 button, const bool pressed) {
        if (!mEnabled) return;

        mMouseStates[button].pressed  = pressed;
        mMouseStates[button].released = !pressed;
    }

    void InputManager::UpdateMousePosition(const f64 x, const f64 y) {
        if (!mEnabled) return;

        mMouseDeltaX = CAST<f32>(x);
        mMouseDeltaY = CAST<f32>(y);

        constexpr f32 deadZone = 2.5f;  // Might need to tweak this, also frame-rate dependent :(
        if (std::abs(mMouseDeltaX) < deadZone) mMouseDeltaX = 0.0f;
        if (std::abs(mMouseDeltaY) < deadZone) mMouseDeltaY = 0.0f;

        mMouseX += x;
        mMouseY += y;
    }

    void InputManager::SetEnabled(const bool enabled) {
        mEnabled = enabled;
    }
}  // namespace Nth::Input