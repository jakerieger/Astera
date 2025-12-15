/*
 *  Filename: InputCodeMap.hpp
 *  This code is part of the Astera core library
 *  Copyright 2025 Jake Rieger
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  In no event and under no legal theory, whether in tort (including negligence),
 *  contract, or otherwise, unless required by applicable law (such as deliberate
 *  and grossly negligent acts) or agreed to in writing, shall any Contributor be
 *  liable for any damages, including any direct, indirect, special, incidental,
 *  or consequential damages of any character arising as a result of this License or
 *  out of the use or inability to use the software (including but not limited to damages
 *  for loss of goodwill, work stoppage, computer failure or malfunction, or any and
 *  all other commercial damages or losses), even if such Contributor has been advised
 *  of the possibility of such damages.
 */

#pragma once

#include "InputCodes.hpp"

namespace Astera::Input {
    inline static const unordered_map<string, u16> kKeyCodeMap = {
      {"Space", Keys::Space},
      {"Apostrophe", Keys::Apostrophe},
      {"Comma", Keys::Comma},
      {"Minus", Keys::Minus},
      {"Period", Keys::Period},
      {"Slash", Keys::Slash},
      {"Num0", Keys::Num0},
      {"Num1", Keys::Num1},
      {"Num2", Keys::Num2},
      {"Num3", Keys::Num3},
      {"Num4", Keys::Num4},
      {"Num5", Keys::Num5},
      {"Num6", Keys::Num6},
      {"Num7", Keys::Num7},
      {"Num8", Keys::Num8},
      {"Num9", Keys::Num9},
      {"Semicolon", Keys::Semicolon},
      {"Equal", Keys::Equal},
      {"A", Keys::A},
      {"B", Keys::B},
      {"C", Keys::C},
      {"D", Keys::D},
      {"E", Keys::E},
      {"F", Keys::F},
      {"G", Keys::G},
      {"H", Keys::H},
      {"I", Keys::I},
      {"J", Keys::J},
      {"K", Keys::K},
      {"L", Keys::L},
      {"M", Keys::M},
      {"N", Keys::N},
      {"O", Keys::O},
      {"P", Keys::P},
      {"Q", Keys::Q},
      {"R", Keys::R},
      {"S", Keys::S},
      {"T", Keys::T},
      {"U", Keys::U},
      {"V", Keys::V},
      {"W", Keys::W},
      {"X", Keys::X},
      {"Y", Keys::Y},
      {"Z", Keys::Z},
      {"LeftBracket", Keys::LeftBracket},
      {"Backslash", Keys::Backslash},
      {"RightBracket", Keys::RightBracket},
      {"GraveAccent", Keys::GraveAccent},
      {"World1", Keys::World1},
      {"World2", Keys::World2},
      {"Escape", Keys::Escape},
      {"Enter", Keys::Enter},
      {"Tab", Keys::Tab},
      {"Backspace", Keys::Backspace},
      {"Insert", Keys::Insert},
      {"Delete", Keys::Delete},
      {"Right", Keys::Right},
      {"Left", Keys::Left},
      {"Down", Keys::Down},
      {"Up", Keys::Up},
      {"PageUp", Keys::PageUp},
      {"PageDown", Keys::PageDown},
      {"Home", Keys::Home},
      {"End", Keys::End},
      {"CapsLock", Keys::CapsLock},
      {"ScrollLock", Keys::ScrollLock},
      {"NumLock", Keys::NumLock},
      {"PrintScreen", Keys::PrintScreen},
      {"Pause", Keys::Pause},
      {"F1", Keys::F1},
      {"F2", Keys::F2},
      {"F3", Keys::F3},
      {"F4", Keys::F4},
      {"F5", Keys::F5},
      {"F6", Keys::F6},
      {"F7", Keys::F7},
      {"F8", Keys::F8},
      {"F9", Keys::F9},
      {"F10", Keys::F10},
      {"F11", Keys::F11},
      {"F12", Keys::F12},
      {"F13", Keys::F13},
      {"F14", Keys::F14},
      {"F15", Keys::F15},
      {"F16", Keys::F16},
      {"F17", Keys::F17},
      {"F18", Keys::F18},
      {"F19", Keys::F19},
      {"F20", Keys::F20},
      {"F21", Keys::F21},
      {"F22", Keys::F22},
      {"F23", Keys::F23},
      {"F24", Keys::F24},
      {"F25", Keys::F25},
      {"KP0", Keys::KP0},
      {"KP1", Keys::KP1},
      {"KP2", Keys::KP2},
      {"KP3", Keys::KP3},
      {"KP4", Keys::KP4},
      {"KP5", Keys::KP5},
      {"KP6", Keys::KP6},
      {"KP7", Keys::KP7},
      {"KP8", Keys::KP8},
      {"KP9", Keys::KP9},
      {"KPDecimal", Keys::KPDecimal},
      {"KPDivide", Keys::KPDivide},
      {"KPMultiply", Keys::KPMultiply},
      {"KPSubtract", Keys::KPSubtract},
      {"KPAdd", Keys::KPAdd},
      {"KPEnter", Keys::KPEnter},
      {"KPEqual", Keys::KPEqual},
      {"LeftShift", Keys::LeftShift},
      {"LeftControl", Keys::LeftControl},
      {"LeftAlt", Keys::LeftAlt},
      {"LeftSuper", Keys::LeftSuper},
      {"RightShift", Keys::RightShift},
      {"RightControl", Keys::RightControl},
      {"RightAlt", Keys::RightAlt},
      {"RightSuper", Keys::RightSuper},
      {"Menu", Keys::Menu},
    };

    inline static const unordered_map<string, u16> kMouseButtonCodeMap = {
      {"Left", MouseButtons::Left},
      {"Right", MouseButtons::Right},
      {"Middle", MouseButtons::Middle},
      {"Macro4", MouseButtons::Macro4},
      {"Macro5", MouseButtons::Macro5},
      {"Macro6", MouseButtons::Macro6},
      {"Macro7", MouseButtons::Macro7},
      {"Macro8", MouseButtons::Macro8},
    };
}  // namespace Astera::Input