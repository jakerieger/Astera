/// @author: Jake Rieger
/// @created: 12/3/2025.
///

#include "Math.hpp"

#include <random>

namespace Nth {
    f32 Math::Min(sol::object obj, f32 a, f32 b) {
        return (a < b) ? a : b;
    }

    f32 Math::Max(sol::object obj, f32 a, f32 b) {
        return (a > b) ? a : b;
    }

    f32 Math::Clamp(const sol::object& obj, f32 a, f32 b) {
        return (a > b) ? Max(obj, a, b) : Min(obj, a, b);
    }

    f32 Math::Sqrt(sol::object obj, f32 a) {
        return sqrt(a);
    }

    f32 Math::Pow(sol::object obj, f32 a, f32 b) {
        return pow(a, b);
    }

    f32 Math::Sin(sol::object obj, f32 a) {
        return sin(a);
    }

    f32 Math::Cos(sol::object obj, f32 a) {
        return cos(a);
    }

    f32 Math::Tan(sol::object obj, f32 a) {
        return tan(a);
    }

    f32 Math::Floor(sol::object obj, f32 a) {
        return floor(a);
    }

    f32 Math::Ceil(sol::object obj, f32 a) {
        return ceil(a);
    }

    f32 Math::Round(sol::object obj, f32 a) {
        return round(a);
    }

    f32 Math::DegToRad(sol::object obj, f32 a) {
        return a * kPi / 180.f;
    }

    f32 Math::RadToDeg(sol::object obj, f32 a) {
        return a * kPi;
    }

    f32 Math::Random() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<f32> dis(0.0f, 1.0f);
        return dis(gen);
    }

    f32 Math::Lerp(sol::object obj, f32 a, f32 b, f32 t) {
        return a + (b - a) * t;
    }

    void Math::RegisterLuaGlobals(sol::state& lua) {
        lua["Math"]      = lua.create_table();
        auto math        = lua["Math"];
        math["Pi"]       = kPi;
        math["Tau"]      = kTau;
        math["E"]        = kEuler;
        math["Min"]      = Min;
        math["Max"]      = Max;
        math["Clamp"]    = Clamp;
        math["Sqrt"]     = Sqrt;
        math["Pow"]      = Pow;
        math["Sin"]      = Sin;
        math["Cos"]      = Cos;
        math["Tan"]      = Tan;
        math["Floor"]    = Floor;
        math["Ceil"]     = Ceil;
        math["Round"]    = Round;
        math["DegToRad"] = DegToRad;
        math["RadToDeg"] = RadToDeg;
        math["Random"]   = Random;
        math["Lerp"]     = Lerp;
    }
}  // namespace Nth