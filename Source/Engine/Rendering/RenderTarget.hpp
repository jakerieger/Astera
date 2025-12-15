/*
 *  Filename: RenderTarget.hpp
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
#include "Rendering/RenderContext.hpp"

namespace Astera {
    /// @brief Types of render targets
    enum class RenderTargetType {
        Window,      ///< Renders directly to the window's default framebuffer
        Framebuffer  ///< Renders to an off-screen framebuffer (texture)
    };

    /// @brief Configuration for render target creation
    struct RenderTargetConfig {
        RenderTargetType type {RenderTargetType::Window};
        u32 width {800};
        u32 height {600};
        bool enableDepth {true};
        bool enableStencil {false};
    };

    /// @brief Abstract rendering surface that OpenGL can render to
    class RenderTarget {
    public:
        /// @brief Creates a render target with the specified configuration
        /// @param config Render target configuration
        explicit RenderTarget(const RenderTargetConfig& config);

        virtual ~RenderTarget();

        ASTERA_CLASS_PREVENT_MOVES_COPIES(RenderTarget)

        /// @brief Initializes the render target
        /// @return True if initialization succeeded, false otherwise
        bool Initialize();

        /// @brief Shuts down and releases render target resources
        void Shutdown();

        /// @brief Binds this render target for rendering
        void Bind();

        /// @brief Unbinds the current render target
        static void Unbind();

        /// @brief Resizes the render target
        /// @param width New width in pixels
        /// @param height New height in pixels
        void Resize(u32 width, u32 height);

        /// @brief Gets the render context for this target
        /// @return Reference to the render context
        ASTERA_KEEP RenderContext& GetContext() {
            return mContext;
        }

        /// @brief Gets the dimensions of the render target
        /// @param outWidth Output parameter for width
        /// @param outHeight Output parameter for height
        void GetSize(u32& outWidth, u32& outHeight) const {
            outWidth  = mWidth;
            outHeight = mHeight;
        }

        /// @brief Gets the texture ID for framebuffer targets
        /// @return Texture ID, or 0 for window targets
        ASTERA_KEEP GLuint GetTextureID() const {
            return mTextureID;
        }

        /// @brief Gets the type of this render target
        /// @return The render target type
        ASTERA_KEEP RenderTargetType GetType() const {
            return mType;
        }

        /// @brief Checks if the render target is initialized
        /// @return True if initialized, false otherwise
        ASTERA_KEEP bool IsInitialized() const {
            return mInitialized;
        }

    private:
        void CreateFramebuffer();
        void DestroyFramebuffer();

        RenderTargetType mType;
        u32 mWidth;
        u32 mHeight;
        bool mEnableDepth;
        bool mEnableStencil;
        bool mInitialized {false};

        // Framebuffer resources (only used if type == Framebuffer)
        GLuint mFramebufferID {0};
        GLuint mTextureID {0};
        GLuint mDepthStencilID {0};

        RenderContext mContext;
    };
}  // namespace Astera