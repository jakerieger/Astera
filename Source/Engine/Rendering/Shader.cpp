/*
 *  Filename: Shader.cpp
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

#include "Shader.hpp"

namespace Astera {
    Shader::~Shader() = default;

    Shader::Shader(const Shader& other) {
        mProgram = other.mProgram;
    }

    Shader& Shader::operator=(const Shader& other) {
        if (this != &other) { mProgram = other.mProgram; }
        return *this;
    }

    Shader::Shader(Shader&& other) noexcept : mProgram(std::exchange(other.mProgram, 0)) {}

    Shader& Shader::operator=(Shader&& other) noexcept {
        if (this != &other) { mProgram = std::exchange(other.mProgram, 0); }
        return *this;
    }

    shared_ptr<Shader> Shader::FromFile(const fs::path& vertexFile, const fs::path& fragFile) {
        auto shader = make_shared<Shader>();

        N_ASSERT(fs::exists(vertexFile) && fs::exists(fragFile));

        const string vertexSource = IO::ReadString(vertexFile);
        const string fragSource   = IO::ReadString(fragFile);

        N_ASSERT(!vertexSource.empty());
        N_ASSERT(!fragSource.empty());

        shader->CompileShaders(vertexSource.c_str(), fragSource.c_str());

        return shader;
    }

    void Shader::FromMemory(const char* vertexSource, const char* fragSource) {
        N_ASSERT(strlen(vertexSource) > 0);
        N_ASSERT(strlen(fragSource) > 0);

        CompileShaders(vertexSource, fragSource);
    }

    void Shader::Bind() {
        GLCall(glUseProgram, mProgram);
    }

    void Shader::Unbind() const {
        N_UNUSED(this);
        GLCall(glUseProgram, 0);
    }

    void Shader::SetUniform(const char* name, bool val) {
        const auto location = GLCall(glGetUniformLocation, mProgram, name);
        VerifyLocation(location, name);
        GLCall(glUniform1i, location, CAST<i32>(val));
    }

    void Shader::SetUniform(const char* name, i32 val) {
        const auto location = GLCall(glGetUniformLocation, mProgram, name);
        VerifyLocation(location, name);
        GLCall(glUniform1i, location, CAST<i32>(val));
    }

    void Shader::SetUniform(const char* name, f32 val) {
        const auto location = GLCall(glGetUniformLocation, mProgram, name);
        VerifyLocation(location, name);
        GLCall(glUniform1f, location, val);
    }

    void Shader::SetUniform(const char* name, const Vec2& val) {
        const auto location = GLCall(glGetUniformLocation, mProgram, name);
        VerifyLocation(location, name);
        GLCall(glUniform2fv, location, 1, &val[0]);
    }

    void Shader::SetUniform(const char* name, const Vec3& val) {
        const auto location = GLCall(glGetUniformLocation, mProgram, name);
        VerifyLocation(location, name);
        GLCall(glUniform3fv, location, 1, &val[0]);
    }

    void Shader::SetUniform(const char* name, const Vec4& val) {
        const auto location = GLCall(glGetUniformLocation, mProgram, name);
        VerifyLocation(location, name);
        GLCall(glUniform4fv, location, 1, &val[0]);
    }

    void Shader::SetUniform(const char* name, const Mat4& val) {
        const auto location = GLCall(glGetUniformLocation, mProgram, name);
        VerifyLocation(location, name);
        GLCall(glUniformMatrix4fv, location, 1, GL_FALSE, &val[0][0]);
    }

    void Shader::CompileShaders(const char* vertexSource, const char* fragSource) {
        const u32 vertexShader = GLCall(glCreateShader, GL_VERTEX_SHADER);
        GLCall(glShaderSource, vertexShader, 1, &vertexSource, nullptr);
        GLCall(glCompileShader, vertexShader);

        // CHECK VERTEX SHADER COMPILATION
        GLint success;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar infoLog[512];
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            Log::Error("Shader", "Vertex shader compilation failed: {}", infoLog);
        }

        const u32 fragmentShader = GLCall(glCreateShader, GL_FRAGMENT_SHADER);
        GLCall(glShaderSource, fragmentShader, 1, &fragSource, nullptr);
        GLCall(glCompileShader, fragmentShader);

        // CHECK FRAGMENT SHADER COMPILATION
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar infoLog[512];
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            Log::Error("Shader", "Fragment shader compilation failed: {}", infoLog);
        }

        mProgram = GLCall(glCreateProgram);
        GLCall(glAttachShader, mProgram, vertexShader);
        GLCall(glAttachShader, mProgram, fragmentShader);
        GLCall(glLinkProgram, mProgram);

        // CHECK PROGRAM LINKING
        glGetProgramiv(mProgram, GL_LINK_STATUS, &success);
        if (!success) {
            GLchar infoLog[512];
            glGetProgramInfoLog(mProgram, 512, nullptr, infoLog);
            Log::Error("Shader", "Shader program linking failed: {}", infoLog);
        }

        GLCall(glValidateProgram, mProgram);

        // CHECK PROGRAM VALIDATION
        glGetProgramiv(mProgram, GL_VALIDATE_STATUS, &success);
        if (!success) {
            GLchar infoLog[512];
            glGetProgramInfoLog(mProgram, 512, nullptr, infoLog);
            Log::Error("Shader", "Shader program validation failed: {}", infoLog);
        }

        GLCall(glDeleteShader, vertexShader);
        GLCall(glDeleteShader, fragmentShader);
    }

    void Shader::Destroy() {
        GLCall(glDeleteProgram, mProgram);
    }

    void Shader::VerifyLocation(i32 location, const char* name) {
        if (location < 0) {
            Log::Critical("Shader", "Could not find uniform `{}` in shader program {}", name, mProgram);
            throw OpenGLException {};
        }
    }
}  // namespace Astera