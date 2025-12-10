/*
 *  Filename: RenderContext.cpp
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

#include "RenderContext.hpp"
#include "Log.hpp"
#include "GLUtils.hpp"

namespace Nth {
    bool RenderContext::Initialize(u32 width, u32 height) {
        if (mInitialized) return true;

        mWidth  = width;
        mHeight = height;

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            Log::Critical("RenderContext", "Failed to load GLAD");
            return false;
        }

        GLCall(glViewport, 0, 0, (i32)mWidth, (i32)mHeight);

        // Enable depth testing (common OpenGL setup)
        GLCall(glEnable, GL_DEPTH_TEST);

        // Enable blending for transparency
        GLCall(glEnable, GL_BLEND);
        GLCall(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        mCommandQueue.Reserve(1000);
        mInitialized = true;

        return true;
    }

    void RenderContext::Shutdown() const {
        N_UNUSED(this);
    }

    void RenderContext::BeginFrame() {
        N_ASSERT(mInitialized);
        Submit(ClearCommand {{0.08f, 0.08f, 0.08f, 1.0f}, true, false});
    }

    void RenderContext::EndFrame() {
        N_ASSERT(mInitialized);
        mCommandQueue.ExecuteQueue();
    }

    void RenderContext::Resize(u32 width, u32 height) {
        N_ASSERT(mInitialized);
        mWidth  = width;
        mHeight = height;
        Submit(SetViewportCommand {0, 0, width, height});
    }
}  // namespace Nth