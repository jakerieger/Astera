/*
 *  Filename: Window.hpp
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

#pragma once

#include "EngineCommon.hpp"
#include "Clock.hpp"
#include "Input.hpp"

namespace Astera {
    /// @brief Configuration for window creation
    struct WindowConfig {
        string title {"Astera Window"};
        u32 width {800};
        u32 height {600};
        bool vsync {false};
        bool resizable {true};
        bool decorated {true};
    };

    /// @brief Base class for platform window management
    class Window {
    public:
        /// @brief Creates a window with the specified configuration
        /// @param config Window configuration parameters
        explicit Window(const WindowConfig& config = {});

        virtual ~Window();

        ASTERA_CLASS_PREVENT_MOVES_COPIES(Window)

        /// @brief Main window loop - calls lifecycle methods
        void Run();

        /// @brief Requests the window to close
        void Close();

        /// @brief Sets the window title
        /// @param title New window title
        void SetTitle(const string& title);

        /// @brief Resizes the window
        /// @param width New width in pixels
        /// @param height New height in pixels
        void Resize(u32 width, u32 height);

        /// @brief Sets the window icon
        /// @param filename Path to the icon image file
        void SetIcon(const Path& filename);

        // Getters

        /// @brief Gets the GLFW window handle
        /// @return Pointer to the GLFWwindow
        ASTERA_KEEP GLFWwindow* GetHandle() const {
            return mWindow;
        }

        /// @brief Gets the current window dimensions
        /// @param outWidth Output parameter for width
        /// @param outHeight Output parameter for height
        void GetSize(u32& outWidth, u32& outHeight) const {
            outWidth  = mWidth;
            outHeight = mHeight;
        }

        /// @brief Gets the window title
        /// @return Current window title
        ASTERA_KEEP const string& GetTitle() const {
            return mTitle;
        }

        /// @brief Checks if the window is currently running
        /// @return True if running, false otherwise
        ASTERA_KEEP bool IsRunning() const {
            return mRunning;
        }

        /// @brief Gets the input manager for this window
        /// @return Reference to the input manager
        ASTERA_KEEP InputManager& GetInputManager() {
            return mInputManager;
        }

        /// @brief Gets the clock for this window
        /// @return Reference to the clock
        ASTERA_KEEP const Clock& GetClock() const {
            return mClock;
        }

    protected:
        // Lifecycle hooks - override these in derived classes

        /// @brief Called once when the window is initialized
        virtual void OnAwake() {}

        /// @brief Called every frame to update logic
        /// @param clock Reference to the game clock
        virtual void OnUpdate(const Clock& clock) {
            ASTERA_UNUSED(clock);
        }

        /// @brief Called after all updates for the frame
        virtual void OnLateUpdate() {}

        /// @brief Called when the window is closing
        virtual void OnDestroyed() {}

        // Input callbacks - override these to handle input

        /// @brief Called when the window is resized
        /// @param width New width in pixels
        /// @param height New height in pixels
        virtual void OnResize(u32 width, u32 height) {
            ASTERA_UNUSED(width);
            ASTERA_UNUSED(height);
        }

        /// @brief Called when a key is pressed
        /// @param keyCode Key code of the pressed key
        virtual void OnKeyDown(u32 keyCode) {
            ASTERA_UNUSED(keyCode);
        }

        /// @brief Called when a key is released
        /// @param keyCode Key code of the released key
        virtual void OnKeyUp(u32 keyCode) {
            ASTERA_UNUSED(keyCode);
        }

        /// @brief Called continuously while a key is held
        /// @param keyCode Key code of the held key
        virtual void OnKey(u32 keyCode) {
            ASTERA_UNUSED(keyCode);
        }

        /// @brief Called when a mouse button is pressed
        /// @param button Mouse button code
        virtual void OnMouseButtonDown(u32 button) {
            ASTERA_UNUSED(button);
        }

        /// @brief Called when a mouse button is released
        /// @param button Mouse button code
        virtual void OnMouseButtonUp(u32 button) {
            ASTERA_UNUSED(button);
        }

        /// @brief Called continuously while a mouse button is held
        /// @param button Mouse button code
        virtual void OnMouseButton(u32 button) {
            ASTERA_UNUSED(button);
        }

        /// @brief Called when the mouse cursor moves
        /// @param dX Horizontal movement delta
        /// @param dY Vertical movement delta
        virtual void OnMouseMove(f64 dX, f64 dY) {
            ASTERA_UNUSED(dX);
            ASTERA_UNUSED(dY);
        }

        /// @brief Called when the mouse wheel is scrolled
        /// @param dX Horizontal scroll delta
        /// @param dY Vertical scroll delta
        virtual void OnMouseScroll(f64 dX, f64 dY) {
            ASTERA_UNUSED(dX);
            ASTERA_UNUSED(dY);
        }

    private:
        // GLFW callbacks
        static void GLFWResizeCallback(GLFWwindow* window, i32 width, i32 height);
        static void GLFWKeyCallback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods);
        static void GLFWMouseButtonCallback(GLFWwindow* window, i32 button, i32 action, i32 mods);
        static void GLFWMouseCursorCallback(GLFWwindow* window, f64 xpos, f64 ypos);
        static void GLFWMouseScrollCallback(GLFWwindow* window, f64 xoffset, f64 yoffset);

        bool Initialize();
        void Shutdown();

    protected:
        GLFWwindow* mWindow {nullptr};
        string mTitle;
        u32 mWidth;
        u32 mHeight;
        bool mRunning {false};
        bool mVsync;

        Clock mClock;
        InputManager mInputManager;
    };
}  // namespace Astera