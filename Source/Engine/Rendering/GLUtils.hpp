/*
 *  Filename: GLUtils.hpp
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

#include "EngineCommon.hpp"

namespace Nth {
    struct OpenGLException final : public std::runtime_error {
        OpenGLException() : std::runtime_error("OpenGLException was thrown") {}
    };

    // Convert OpenGL error code to string
    constexpr std::string_view GLErrorToString(GLenum error) {
        switch (error) {
            case GL_NO_ERROR:
                return "GL_NO_ERROR";
            case GL_INVALID_ENUM:
                return "GL_INVALID_ENUM";
            case GL_INVALID_VALUE:
                return "GL_INVALID_VALUE";
            case GL_INVALID_OPERATION:
                return "GL_INVALID_OPERATION";
            case GL_STACK_OVERFLOW:
                return "GL_STACK_OVERFLOW";
            case GL_STACK_UNDERFLOW:
                return "GL_STACK_UNDERFLOW";
            case GL_OUT_OF_MEMORY:
                return "GL_OUT_OF_MEMORY";
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                return "GL_INVALID_FRAMEBUFFER_OPERATION";
            default:
                return "UNKNOWN_ERROR";
        }
    }

    // Check for OpenGL errors and report them
    inline void GLCheckError(std::string_view function_name, const std::source_location& location) {
        GLenum error;
        bool hasError = false;

        while ((error = glGetError()) != GL_NO_ERROR) {
            hasError = true;
            auto msg = fmt::format("[OpenGL Error] {}\n"
                                   "  Function: {}\n"
                                   "  File: {}:{}\n"
                                   "  In function: {}\n",
                                   GLErrorToString(error),
                                   function_name,
                                   location.file_name(),
                                   location.line(),
                                   location.function_name());
            fprintf(stderr, "%s\n", msg.c_str());
        }

        if (hasError) { throw OpenGLException(); }
    }

    // Helper to call and check - void version
    template<typename Func, typename... Args>
        requires std::is_void_v<std::invoke_result_t<Func, Args...>>
    void GLCallImpl(const char* func_name, const std::source_location& loc, Func&& func, Args&&... args) {
        while (glGetError() != GL_NO_ERROR)
            ;
        std::forward<Func>(func)(std::forward<Args>(args)...);
        GLCheckError(func_name, loc);
    }

    // Helper to call and check - non-void version
    template<typename Func, typename... Args>
        requires(!std::is_void_v<std::invoke_result_t<Func, Args...>>)
    auto GLCallImpl(const char* func_name, const std::source_location& loc, Func&& func, Args&&... args) {
        while (glGetError() != GL_NO_ERROR)
            ;
        auto result = std::forward<Func>(func)(std::forward<Args>(args)...);
        GLCheckError(func_name, loc);
        return result;
    }

// Macro that works for both void and non-void
#define GLCall(func, ...) GLCallImpl(#func, std::source_location::current(), func, ##__VA_ARGS__)
}  // namespace Nth