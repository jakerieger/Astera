// Author: Jake Rieger
// Created: 11/27/25.
//

#include "Game.hpp"
#include "Log.hpp"

namespace N {
    using N::Log;

    void IGame::Run() {
        mRunning = Initialize();
        if (!mRunning) return;
        OnAwake();
        {
            // Main loop
            while (mRunning && !glfwWindowShouldClose(mWindow)) {
                mClock.Tick();
                OnUpdate(mClock);
                Render();
                glfwSwapBuffers(mWindow);
                OnLateUpdate();
                glfwPollEvents();
            }

            mRunning = false;
        }
        OnDestroyed();
        Shutdown();
    }

    void IGame::Quit() {
        mRunning = false;
    }

    void IGame::ToggleFullscreen() {
        mFullscreen = !mFullscreen;
        throw N_NOT_IMPLEMENTED;
    }

    void IGame::SetTitle(const string& title) {
        mTitle = title;
        glfwSetWindowTitle(mWindow, title.c_str());
    }

    void IGame::Resize(u32 width, u32 height) {
        mWidth  = width;
        mHeight = height;
        glfwSetWindowSize(mWindow, (s32)width, (s32)height);
    }

    bool IGame::Initialize() {
        Log::Initialize();

        if (!glfwInit()) {
            Log::Critical("Failed to initialize GLFW");
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        mWindow = glfwCreateWindow((s32)mWidth, (s32)mHeight, mTitle.c_str(), nullptr, nullptr);
        if (!mWindow) {
            glfwTerminate();
            Log::Critical("Failed to create GLFW window");
            return false;
        }

        glfwMakeContextCurrent(mWindow);
        glfwSetWindowUserPointer(mWindow, this);

        // SetupOpenGL();

        glfwSetFramebufferSizeCallback(mWindow, GLFWResizeCallback);
        glfwSetKeyCallback(mWindow, GLFWKeyCallback);
        glfwSetMouseButtonCallback(mWindow, GLFWMouseButtonCallback);
        glfwSetCursorPosCallback(mWindow, GLFWMouseCursorCallback);
        glfwSetScrollCallback(mWindow, GLFWMouseScrollCallback);

        // glfwSwapInterval(1);

        Log::Info("Successfully initialized game instance");

        return true;
    }

    void IGame::Shutdown() const {
        Log::Shutdown();

        if (mWindow) glfwDestroyWindow(mWindow);
        glfwTerminate();
    }

    void IGame::Render() const {}

    void IGame::GLFWResizeCallback(GLFWwindow* window, s32 width, s32 height) {
        auto* game = CAST<IGame*>(glfwGetWindowUserPointer(window));
        if (game) { game->OnResize(width, height); }
        // glViewport(0, 0, width, height);
    }

    void IGame::GLFWKeyCallback(GLFWwindow* window, s32 key, s32 scancode, s32 action, s32 mods) {
        N_UNUSED(scancode);
        N_UNUSED(mods);

        auto* game = CAST<IGame*>(glfwGetWindowUserPointer(window));
        if (game) {
            game->OnKey(key);
            if (action == GLFW_PRESS) game->OnKeyDown(key);
            else if (action == GLFW_RELEASE) game->OnKeyUp(key);
        }
    }

    void IGame::GLFWMouseButtonCallback(GLFWwindow* window, s32 button, s32 action, s32 mods) {
        N_UNUSED(mods);

        auto* game = CAST<IGame*>(glfwGetWindowUserPointer(window));
        if (game) {
            game->OnMouseButton(button);
            if (action == GLFW_PRESS) game->OnMouseButtonDown(button);
            else if (action == GLFW_RELEASE) game->OnMouseButtonUp(button);
        }
    }

    void IGame::GLFWMouseCursorCallback(GLFWwindow* window, f64 xpos, f64 ypos) {
        auto* game = CAST<IGame*>(glfwGetWindowUserPointer(window));
        if (game) { game->OnMouseMove(xpos, ypos); }
    }

    void IGame::GLFWMouseScrollCallback(GLFWwindow* window, f64 xoffset, f64 yoffset) {
        auto* game = CAST<IGame*>(glfwGetWindowUserPointer(window));
        if (game) { game->OnMouseScroll(xoffset, yoffset); }
    }
}  // namespace N