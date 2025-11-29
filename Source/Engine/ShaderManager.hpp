// Author: Jake Rieger
// Created: 11/29/25.
//

#pragma once

#include "CommonPCH.hpp"
#include "Rendering/Shader.hpp"

namespace Nth {
    namespace Shaders {
        inline const string Sprite = "sprite";
    }

    class ShaderManager {
    public:
        static void Initialize();
        static void Shutdown();

        N_ND static Shader* GetShader(const string& name);

    private:
        static unordered_map<string, Shader> sCache;
        static shared_ptr<ShaderManager> sManager;
        static shared_ptr<ShaderManager> GetManager();
    };
}  // namespace Nth
