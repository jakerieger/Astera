// Author: Jake Rieger
// Created: 11/29/25.
//

#pragma once

#include "CommonPCH.hpp"

namespace Nth {
    class Shader {
    public:
        Shader()  = default;
        ~Shader() = default;

        void FromFile(const fs::path& vertexFile, const fs::path& fragFile);
        void FromMemory(const char* vertexSource, const char* fragSource);

        // Generic template definition
        // template<typename T>
        // void SetUniform(T) {}

    private:
        GLuint mProgram {0};

        void CompileShaders();
    };
}  // namespace Nth
