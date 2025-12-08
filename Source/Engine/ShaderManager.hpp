/// @author Jake Rieger
/// @created 11/29/25
///

#pragma once

#include "Common/CommonPCH.hpp"
#include "Rendering/Shader.hpp"

namespace Nth {
    namespace Shaders {
        inline constexpr std::string_view Sprite = "sprite";
    }

    class ShaderManager {
    public:
        static void Initialize();
        static void Shutdown();

        N_ND static shared_ptr<Shader> GetShader(std::string_view name);

    private:
        static unordered_map<string, shared_ptr<Shader>> sCache;
        static shared_ptr<ShaderManager> sManager;
        static shared_ptr<ShaderManager> GetManager();
    };
}  // namespace Nth
