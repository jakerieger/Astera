/// @author Jake Rieger
/// @created 11/29/25
///

#pragma once

#include "Common/CommonPCH.hpp"

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