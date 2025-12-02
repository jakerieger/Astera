/// @author Jake Rieger
/// @created 11/29/25
///

#pragma once

#include "CommonPCH.hpp"
#include "GLUtils.hpp"
#include "Log.hpp"

namespace Nth {
    class Shader {
        friend class ShaderManager;

    public:
        Shader() = default;
        ~Shader();

        Shader(const Shader& other);
        Shader& operator=(const Shader& other);

        Shader(Shader&& other) noexcept;
        Shader& operator=(Shader&& other) noexcept;

        void FromFile(const fs::path& vertexFile, const fs::path& fragFile);
        void FromMemory(const char* vertexSource, const char* fragSource);

        void Bind();
        void Unbind() const;

        void SetUniform(const char* name, bool val);
        void SetUniform(const char* name, i32 val);
        void SetUniform(const char* name, f32 val);
        void SetUniform(const char* name, const Vec2& val);
        void SetUniform(const char* name, const Vec3& val);
        void SetUniform(const char* name, const Vec4& val);
        void SetUniform(const char* name, const Mat4& val);

        N_ND GLuint GetProgramID() const {
            return mProgram;
        }

    private:
        GLuint mProgram {0};

        void CompileShaders(const char* vertexSource, const char* fragSource);
        void Destroy();

        inline void VerifyLocation(i32 location, const char* name);
    };
}  // namespace Nth
