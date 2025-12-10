/// @author Jake Rieger
/// @created 11/28/25
///

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