/*
 *  Filename: SceneParser.hpp
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
#include "SceneDescriptor.hpp"

namespace Astera {
    struct SceneDescriptor;
    class SceneState;
    class ScriptEngine;

    /// @brief Utility class for converting between scene states and descriptors, and handling serialization
    class SceneParser {
    public:
        /// @brief Converts a scene state to a scene descriptor
        /// @param state The scene state to convert from
        /// @param outDescriptor The descriptor to populate with state data
        static void StateToDescriptor(const SceneState& state, SceneDescriptor& outDescriptor);

        /// @brief Converts a scene descriptor to a scene state
        /// @param descriptor The descriptor to convert from
        /// @param outState The scene state to populate with descriptor data
        /// @param scriptEngine Reference to the script engine for initializing scripts
        static void
        DescriptorToState(const SceneDescriptor& descriptor, SceneState& outState, ScriptEngine& scriptEngine);

        /// @brief Serializes a scene descriptor to disk
        /// @param descriptor The descriptor to serialize
        static void SerializeDescriptor(const SceneDescriptor& descriptor);

        /// @brief Deserializes a scene descriptor from a file
        /// @param filename Path to the file containing the serialized descriptor
        /// @param outDescriptor The descriptor to populate with deserialized data
        static void DeserializeDescriptor(const fs::path& filename, SceneDescriptor& outDescriptor);

        /// @brief Deserializes a scene descriptor from a string source
        /// @param source String containing the serialized descriptor data
        /// @param outDescriptor The descriptor to populate with deserialized data
        static void DeserializeDescriptor(const string& source, SceneDescriptor& outDescriptor);
    };
}  // namespace Astera