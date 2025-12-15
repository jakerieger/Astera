/*
 *  Filename: EngineConfig.hpp
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

#include "EngineCommon.hpp"
#include "InputCodeMap.hpp"

#define MINI_CASE_SENSITIVE 1
#include <mini/ini.h>

namespace Astera {
    /// @brief Stores key and mouse button codes an action maps to
    struct ActionMap {
        /// @brief Key codes this action maps to
        vector<u32> keys;
        /// @brief Mouse buttons this action maps to
        vector<u32> mouseButtons;
    };

    /// @brief Stores the mouse and/or joystick code an axis maps to
    struct AxisMap {
        bool hasMouse {false};
        bool hasJoystick {false};
        /// @brief Mouse axis this axis maps to
        u32 mouseAxis {0};
        /// @brief Joystick axis this axis maps to
        u32 joystickAxis {0};
    };

    /// @brief Holds the custom input map defined by the game
    struct InputMap {
        unordered_map<string, ActionMap> actions;
        unordered_map<string, AxisMap> axes;

        inline bool Load(const Path& filename) {
            using namespace mINI;
            const INIFile iniFile(filename);
            INIStructure ini;
            if (!iniFile.read(ini)) {
                return false;
            }

            auto SplitBindings = [](const string& value) -> vector<string> {
                vector<std::string> result;
                std::stringstream ss(value);
                string token;
                while (std::getline(ss, token, ',')) {
                    result.push_back(token);
                }
                return result;
            };

            // Iterate Actions
            for (auto const& [actionName, bindings] : ini["Actions"]) {
                const auto split = SplitBindings(bindings);
                ActionMap map;

                for (const auto& binding : split) {
                    if (binding.find("Key.") != string::npos) {
                        const auto keyCodeStr = binding.substr(4, binding.length());
                        const auto keyCode    = Input::kKeyCodeMap.at(keyCodeStr);
                        map.keys.push_back(keyCode);
                    } else if (binding.find("Mouse.") != string::npos) {
                        const auto mouseCodeStr = binding.substr(6, binding.length());
                        const auto mouseCode    = Input::kMouseButtonCodeMap.at(mouseCodeStr);
                        map.mouseButtons.push_back(mouseCode);
                    }
                }

                actions[actionName] = map;
            }

            // TODO: Iterate Axes
            for (auto const& [axisName, binding] : ini["Axes"]) {
                // axisName = "Aim"
                // binding = "Mouse.X"
            }

            return true;
        }
    };

    struct EngineConfig {};
}  // namespace Astera