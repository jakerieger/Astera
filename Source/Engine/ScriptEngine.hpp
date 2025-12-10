/*
 *  Filename: ScriptEngine.hpp
 *  This code is part of the Nth Engine core library
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

#include "Common/CommonPCH.hpp"

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include "Clock.hpp"
#include "Components/Transform.hpp"

namespace Nth {
    struct BehaviorEntity;

    /// @brief Trait struct for extracting Lua type information from C++ types
    /// @tparam T The C++ type to extract Lua information from
    template<typename T, typename = void>
    struct LuaTypeTraits {
        /// @brief The name of the type as it appears in Lua
        static constexpr std::string_view typeName = T::kLuaTypeName;

        /// @brief Registers the type's members with Lua
        /// @param usertype The sol::usertype to register members with
        static void RegisterMembers(sol::usertype<T>& usertype) {
            T::RegisterMembers(usertype);
        }
    };

    /// @brief Registration helper struct for Lua types
    /// @tparam T The C++ type to register
    template<typename T>
    struct LuaTypeRegistration {};

    /// @brief Interface for types that can be registered with Lua
    struct LuaRegisterable {
        virtual ~LuaRegisterable() = default;

        /// @brief Registers this type with the Lua state
        /// @param lua Reference to the Lua state
        virtual void RegisterWithLua(sol::state& lua) const = 0;
    };

    /// @brief Concrete implementation of Lua type registration
    /// @tparam T The C++ type to register with Lua
    template<typename T>
    struct LuaRegistry final : LuaRegisterable {
        /// @brief The name of the type as it appears in Lua
        static constexpr std::string_view typeName = LuaTypeTraits<T>::typeName;

        /// @brief Registers this type with the Lua state
        /// @param lua Reference to the Lua state
        void RegisterWithLua(sol::state& lua) const override {
            auto usertype = lua.new_usertype<T>(std::string {typeName});
            LuaTypeTraits<T>::RegisterMembers(usertype);
        }
    };

    /// @brief Context data for a behavior script, including its environment and lifecycle callbacks
    struct BehaviorScriptContext {
        /// @brief Isolated Lua environment for this script
        sol::environment env;
        /// @brief Lua function called when the behavior is initialized
        sol::protected_function OnAwake;
        /// @brief Lua function called every frame during update
        sol::protected_function OnUpdate;
        /// @brief Lua function called after all updates for the frame
        sol::protected_function OnLateUpdate;
        /// @brief Lua function called when the behavior is destroyed
        sol::protected_function OnDestroyed;
    };

    /// @brief Types of scripts supported by the engine
    enum class ScriptType : u8 {
        /// @brief Entity behavior script
        Behavior,
        /// @brief Total number of script types
        ScriptTypeCount,
    };

    /// @brief Manages Lua script execution and lifecycle for the game engine
    class ScriptEngine {
    public:
        /// @brief Type alias for script identifiers
        using ScriptID = u64;

        ScriptEngine()  = default;
        ~ScriptEngine() = default;

        N_CLASS_PREVENT_MOVES_COPIES(ScriptEngine)

        /// @brief Initializes the script engine and Lua state
        void Initialize();

        /// @brief Loads a script from source code
        /// @param source The Lua source code to load
        /// @param scriptId Unique identifier for this script
        /// @param type The type of script being loaded (default: Behavior)
        void LoadScript(const string& source, ScriptID scriptId, ScriptType type = ScriptType::Behavior);

        /// @brief Loads a script from compiled bytecode
        /// @param bytecode The compiled Lua bytecode to load
        /// @param scriptId Unique identifier for this script
        /// @param type The type of script being loaded (default: Behavior)
        void LoadScript(const vector<u8>& bytecode, ScriptID scriptId, ScriptType type = ScriptType::Behavior);

        /// @brief Calls the OnAwake callback for a behavior script
        /// @param id The script ID to execute
        /// @param entity The entity associated with this behavior
        void CallAwakeBehavior(ScriptID id, const BehaviorEntity& entity);

        /// @brief Calls the OnUpdate callback for a behavior script
        /// @param id The script ID to execute
        /// @param entity The entity associated with this behavior
        /// @param clock Reference to the game clock for timing information
        void CallUpdateBehavior(ScriptID id, const BehaviorEntity& entity, const Clock& clock);

        /// @brief Calls the OnLateUpdate callback for a behavior script
        /// @param id The script ID to execute
        /// @param entity The entity associated with this behavior
        void CallLateUpdateBehavior(ScriptID id, const BehaviorEntity& entity);

        /// @brief Calls the OnDestroyed callback for a behavior script
        /// @param id The script ID to execute
        /// @param entity The entity associated with this behavior
        void CallDestroyedBehavior(ScriptID id, const BehaviorEntity& entity);

        /// @brief Executes a Lua script file
        /// @param filename Path to the Lua file to execute
        void ExecuteFile(const fs::path& filename);

        /// @brief Executes Lua source code
        /// @param source The Lua source code to execute
        void Execute(const string& source);

        /// @brief Registers a C++ type with the Lua environment
        /// @tparam T The C++ type to register
        template<typename T>
        void RegisterType() {
            LuaRegistry<T> {}.RegisterWithLua(mLua);
        }

        /// @brief Registers multiple C++ types with the Lua environment
        /// @tparam Types The C++ types to register
        template<typename... Types>
        void RegisterTypes() {
            (RegisterType<Types>(), ...);
        }

        /// @brief Gets the underlying Lua state
        /// @return Reference to the sol::state object
        sol::state& GetLuaState() {
            return mLua;
        }

        /// @brief Checks if the script engine has been initialized
        /// @return True if initialized, false otherwise
        bool IsInitialized() const {
            return mInitialized;
        }

    private:
        /// @brief Whether the script engine is initialized
        bool mInitialized {false};
        /// @brief The underlying Lua state
        sol::state mLua;
        /// @brief Map of script IDs to their behavior script contexts
        unordered_map<ScriptID, BehaviorScriptContext> mBehaviorScriptContexts;
    };
}  // namespace Nth