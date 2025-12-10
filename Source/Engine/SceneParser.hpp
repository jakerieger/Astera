/// @author Jake Rieger
/// @created 12/2/25
///

#pragma once

#include "Common/CommonPCH.hpp"
#include "SceneDescriptor.hpp"

namespace Nth {
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
}  // namespace Nth