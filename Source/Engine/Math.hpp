/// @author: Jake Rieger
/// @created: 12/3/2025.
///

#pragma once

#include "CommonPCH.hpp"

#include <sol/sol.hpp>

namespace sol {
    class state;
}  // namespace sol

namespace Nth {
    /// @brief Implements standard math operations and defines common constants for use in Lua
    class Math {
    public:
        inline static constexpr f64 kPi    = 3.141592653589793;
        inline static constexpr f64 kTau   = kPi * kPi;
        inline static constexpr f64 kEuler = 2.718281828459045;

        static f32 Min(sol::object obj, f32 a, f32 b);
        static f32 Max(sol::object obj, f32 a, f32 b);
        static f32 Clamp(const sol::object& obj, f32 a, f32 b);
        static f32 Sqrt(sol::object obj, f32 a);
        static f32 Pow(sol::object obj, f32 a, f32 b);
        static f32 Sin(sol::object obj, f32 a);
        static f32 Cos(sol::object obj, f32 a);
        static f32 Tan(sol::object obj, f32 a);
        static f32 Floor(sol::object obj, f32 a);
        static f32 Ceil(sol::object obj, f32 a);
        static f32 Round(sol::object obj, f32 a);
        static f32 DegToRad(sol::object obj, f32 a);
        static f32 RadToDeg(sol::object obj, f32 a);
        static f32 Random();
        static f32 Lerp(sol::object obj, f32 a, f32 b, f32 t);

    private:
        friend class Game;

        static void RegisterLuaGlobals(sol::state& lua);
    };
}  // namespace Nth