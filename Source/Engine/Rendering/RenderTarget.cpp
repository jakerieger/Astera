/*
 *  Filename: RenderTarget.cpp
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

#include "RenderTarget.hpp"
#include "GLUtils.hpp"
#include "Log.hpp"

namespace Astera {
    RenderTarget::RenderTarget(const RenderTargetConfig& config)
        : mType(config.type), mWidth(config.width), mHeight(config.height), mEnableDepth(config.enableDepth),
          mEnableStencil(config.enableStencil) {}

    RenderTarget::~RenderTarget() {
        Shutdown();
    }

    bool RenderTarget::Initialize() {
        if (mInitialized) {
            Log::Warn("RenderTarget", "Already initialized");
            return true;
        }

        // Initialize render context
        if (!mContext.Initialize(mWidth, mHeight)) {
            Log::Error("RenderTarget", "Failed to initialize render context");
            return false;
        }

        // Create framebuffer if not rendering to window
        if (mType == RenderTargetType::Framebuffer) {
            CreateFramebuffer();
        }

        mInitialized = true;
        Log::Debug("RenderTarget",
                   "Initialized {} render target ({}x{})",
                   mType == RenderTargetType::Window ? "window" : "framebuffer",
                   mWidth,
                   mHeight);

        return true;
    }

    void RenderTarget::Shutdown() {
        if (!mInitialized) {
            return;
        }

        if (mType == RenderTargetType::Framebuffer) {
            DestroyFramebuffer();
        }

        mContext.Shutdown();
        mInitialized = false;
    }

    void RenderTarget::Bind() {
        ASTERA_ASSERT(mInitialized);

        if (mType == RenderTargetType::Framebuffer) {
            GLCall(glBindFramebuffer, GL_FRAMEBUFFER, mFramebufferID);
        } else {
            GLCall(glBindFramebuffer, GL_FRAMEBUFFER, 0);
        }

        GLCall(glViewport, 0, 0, (GLsizei)mWidth, (GLsizei)mHeight);
    }

    void RenderTarget::Unbind() {
        GLCall(glBindFramebuffer, GL_FRAMEBUFFER, 0);
    }

    void RenderTarget::Resize(u32 width, u32 height) {
        if (width == mWidth && height == mHeight) {
            return;
        }

        mWidth  = width;
        mHeight = height;

        mContext.Resize(width, height);

        if (mType == RenderTargetType::Framebuffer) {
            DestroyFramebuffer();
            CreateFramebuffer();
        }

        Log::Debug("RenderTarget", "Resized to {}x{}", width, height);
    }

    void RenderTarget::CreateFramebuffer() {
        ASTERA_ASSERT(mType == RenderTargetType::Framebuffer);

        // Create framebuffer
        GLCall(glGenFramebuffers, 1, &mFramebufferID);
        GLCall(glBindFramebuffer, GL_FRAMEBUFFER, mFramebufferID);

        // Create color texture
        GLCall(glGenTextures, 1, &mTextureID);
        GLCall(glBindTexture, GL_TEXTURE_2D, mTextureID);
        GLCall(glTexImage2D,
               GL_TEXTURE_2D,
               0,
               GL_RGBA8,
               (GLsizei)mWidth,
               (GLsizei)mHeight,
               0,
               GL_RGBA,
               GL_UNSIGNED_BYTE,
               nullptr);
        GLCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        GLCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        GLCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        GLCall(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        GLCall(glFramebufferTexture2D, GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextureID, 0);

        // Create depth/stencil buffer if requested
        if (mEnableDepth || mEnableStencil) {
            GLCall(glGenRenderbuffers, 1, &mDepthStencilID);
            GLCall(glBindRenderbuffer, GL_RENDERBUFFER, mDepthStencilID);

            GLenum format;
            GLenum attachment;

            if (mEnableDepth && mEnableStencil) {
                format     = GL_DEPTH24_STENCIL8;
                attachment = GL_DEPTH_STENCIL_ATTACHMENT;
            } else if (mEnableDepth) {
                format     = GL_DEPTH_COMPONENT24;
                attachment = GL_DEPTH_ATTACHMENT;
            } else {
                format     = GL_STENCIL_INDEX8;
                attachment = GL_STENCIL_ATTACHMENT;
            }

            GLCall(glRenderbufferStorage, GL_RENDERBUFFER, format, (GLsizei)mWidth, (GLsizei)mHeight);
            GLCall(glFramebufferRenderbuffer, GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, mDepthStencilID);
        }

        // Check framebuffer completeness
        GLenum status = GLCall(glCheckFramebufferStatus, GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            Log::Error("RenderTarget", "Framebuffer is not complete: {}", status);
        }

        GLCall(glBindFramebuffer, GL_FRAMEBUFFER, 0);
        Log::Debug("RenderTarget", "Created framebuffer ({}x{})", mWidth, mHeight);
    }

    void RenderTarget::DestroyFramebuffer() {
        if (mFramebufferID != 0) {
            GLCall(glDeleteFramebuffers, 1, &mFramebufferID);
            mFramebufferID = 0;
        }

        if (mTextureID != 0) {
            GLCall(glDeleteTextures, 1, &mTextureID);
            mTextureID = 0;
        }

        if (mDepthStencilID != 0) {
            GLCall(glDeleteRenderbuffers, 1, &mDepthStencilID);
            mDepthStencilID = 0;
        }
    }
}  // namespace Astera