/// @author Jake Rieger
/// @created 11/29/25
///

#pragma once

#include "Common/CommonPCH.hpp"
#include "Rendering/Shader.hpp"

namespace Nth {
    /// @brief Predefined shader name constants
    namespace Shaders {
        /// @brief Name identifier for the sprite shader
        inline constexpr std::string_view Sprite = "sprite";
    }  // namespace Shaders

    /// @brief Manages shader resources and provides cached access to compiled shaders
    class ShaderManager {
    public:
        /// @brief Initializes the shader manager and loads default shaders
        static void Initialize();

        /// @brief Shuts down the shader manager and releases all cached shaders
        static void Shutdown();

        /// @brief Retrieves a shader by name from the cache
        /// @param name The name identifier of the shader to retrieve
        /// @return Shared pointer to the requested shader, or nullptr if not found
        N_ND static shared_ptr<Shader> GetShader(std::string_view name);

    private:
        /// @brief Cache mapping shader names to their compiled shader objects
        static unordered_map<string, shared_ptr<Shader>> sCache;

        /// @brief Singleton instance of the shader manager
        static shared_ptr<ShaderManager> sManager;

        /// @brief Retrieves or creates the singleton shader manager instance
        /// @return Shared pointer to the shader manager instance
        static shared_ptr<ShaderManager> GetManager();
    };
}  // namespace Nth