/*
 *  Filename: Window.cpp
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
 */

#include "Window.hpp"
#include "Log.hpp"

#include <stb_image.h>

namespace Astera {
    Window::Window(const WindowConfig& config)
        : mTitle(config.title), mWidth(config.width), mHeight(config.height), mVsync(config.vsync) {}

    Window::~Window() {
        Shutdown();
    }

    void Window::Run() {
        mRunning = Initialize();
        if (!mRunning)
            return;

        OnAwake();

        while (mRunning && !glfwWindowShouldClose(mWindow)) {
            mClock.Tick();
            OnUpdate(mClock);
            OnLateUpdate();
            glfwPollEvents();
        }

        OnDestroyed();
        Shutdown();
    }

    void Window::Close() {
        mRunning = false;
    }

    void Window::SetTitle(const string& title) {
        mTitle = title;
        glfwSetWindowTitle(mWindow, title.c_str());
    }

    void Window::Resize(u32 width, u32 height) {
        glfwSetWindowSize(mWindow, (i32)width, (i32)height);
    }

    void Window::SetIcon(const Path& filename) {
        i32 width, height, channels;
        u8* pixels = stbi_load(filename.string().c_str(), &width, &height, &channels, 4);

        if (pixels) {
            GLFWimage icon;
            icon.width  = width;
            icon.height = height;
            icon.pixels = pixels;
            glfwSetWindowIcon(mWindow, 1, &icon);
            stbi_image_free(pixels);
        } else {
            Log::Error("Window", "Failed to load window icon: {}", filename.string());
        }
    }

    bool Window::Initialize() {
        if (!glfwInit()) {
            Log::Critical("Window", "Failed to initialize GLFW");
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        mWindow = glfwCreateWindow((i32)mWidth, (i32)mHeight, mTitle.c_str(), nullptr, nullptr);
        if (!mWindow) {
            glfwTerminate();
            Log::Critical("Window", "Failed to create GLFW window");
            return false;
        }

        glfwMakeContextCurrent(mWindow);
        glfwSetWindowUserPointer(mWindow, this);

        // Set up callbacks
        glfwSetFramebufferSizeCallback(mWindow, GLFWResizeCallback);
        glfwSetKeyCallback(mWindow, GLFWKeyCallback);
        glfwSetMouseButtonCallback(mWindow, GLFWMouseButtonCallback);
        glfwSetCursorPosCallback(mWindow, GLFWMouseCursorCallback);
        glfwSetScrollCallback(mWindow, GLFWMouseScrollCallback);

        if (mVsync)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        // Initialize OpenGL
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            Log::Critical("Window", "Failed to load GLAD");
            glfwDestroyWindow(mWindow);
            glfwTerminate();
            return false;
        }

        Log::Debug("Window", "Window initialized: {}x{}", mWidth, mHeight);
        return true;
    }

    void Window::Shutdown() {
        if (mWindow) {
            glfwDestroyWindow(mWindow);
            mWindow = nullptr;
        }
        glfwTerminate();
    }

    // GLFW Callbacks

    void Window::GLFWResizeCallback(GLFWwindow* window, i32 width, i32 height) {
        auto* self = CAST<Window*>(glfwGetWindowUserPointer(window));
        if (self) {
            self->mWidth  = width;
            self->mHeight = height;
            self->OnResize(width, height);
        }
    }

    void Window::GLFWKeyCallback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods) {
        ASTERA_UNUSED(scancode);
        ASTERA_UNUSED(mods);

        auto* self = CAST<Window*>(glfwGetWindowUserPointer(window));
        if (self) {
            self->OnKey(key);
            if (action == GLFW_PRESS) {
                self->mInputManager.UpdateKeyState(key, true);
                self->OnKeyDown(key);
            } else if (action == GLFW_RELEASE) {
                self->mInputManager.UpdateKeyState(key, false);
                self->OnKeyUp(key);
            }
        }
    }

    void Window::GLFWMouseButtonCallback(GLFWwindow* window, i32 button, i32 action, i32 mods) {
        ASTERA_UNUSED(mods);

        auto* self = CAST<Window*>(glfwGetWindowUserPointer(window));
        if (self) {
            self->OnMouseButton(button);
            if (action == GLFW_PRESS) {
                self->mInputManager.UpdateMouseButtonState(button, true);
                self->OnMouseButtonDown(button);
            } else if (action == GLFW_RELEASE) {
                self->mInputManager.UpdateMouseButtonState(button, false);
                self->OnMouseButtonUp(button);
            }
        }
    }

    void Window::GLFWMouseCursorCallback(GLFWwindow* window, f64 xpos, f64 ypos) {
        auto* self = CAST<Window*>(glfwGetWindowUserPointer(window));
        if (self) {
            self->mInputManager.UpdateMousePosition(xpos, ypos);
            self->OnMouseMove(xpos, ypos);
        }
    }

    void Window::GLFWMouseScrollCallback(GLFWwindow* window, f64 xoffset, f64 yoffset) {
        auto* self = CAST<Window*>(glfwGetWindowUserPointer(window));
        if (self) {
            self->OnMouseScroll(xoffset, yoffset);
        }
    }
}  // namespace Astera