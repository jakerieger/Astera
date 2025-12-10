/*
 *  Filename: Scene.hpp
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

#include "Clock.hpp"
#include "Common/CommonPCH.hpp"
#include "SceneState.hpp"
#include "ScriptEngine.hpp"
#include "Rendering/RenderContext.hpp"

namespace Nth {
    /// @brief Represents a game scene with lifecycle management and rendering capabilities
    class Scene {
        friend class Game;

    public:
        Scene() = default;
        ~Scene();

        N_CLASS_PREVENT_MOVES_COPIES(Scene)

        /// @brief Called when the scene is first initialized
        /// @param scriptEngine Reference to the script engine for executing scene scripts
        void Awake(ScriptEngine& scriptEngine);

        /// @brief Called every frame to update scene logic
        /// @param scriptEngine Reference to the script engine for executing update scripts
        /// @param clock Reference to the game clock for timing information
        void Update(ScriptEngine& scriptEngine, const Clock& clock);

        /// @brief Called after all Update calls have completed for the frame
        /// @param scriptEngine Reference to the script engine for executing late update scripts
        void LateUpdate(ScriptEngine& scriptEngine);

        /// @brief Called when the scene is being destroyed
        /// @param scriptEngine Reference to the script engine for executing cleanup scripts
        void Destroyed(ScriptEngine& scriptEngine);

        /// @brief Renders the scene to the screen
        /// @param context Reference to the render context for drawing operations
        void Render(RenderContext& context);

        /// @brief Loads a scene from a file
        /// @param filename Path to the scene file to load
        /// @param scriptEngine Reference to the script engine for initializing scene scripts
        void Load(const fs::path& filename, ScriptEngine& scriptEngine);

        /// @brief Loads a scene from a string source
        /// @param source String containing the scene data
        /// @param scriptEngine Reference to the script engine for initializing scene scripts
        void Load(const string& source, ScriptEngine& scriptEngine);

        /// @brief Gets the current state of the scene
        /// @return Reference to the scene state object
        N_ND SceneState& GetState() {
            return mState;
        }

    private:
        /// @brief Internal state data for the scene
        SceneState mState;
    };
}  // namespace Nth