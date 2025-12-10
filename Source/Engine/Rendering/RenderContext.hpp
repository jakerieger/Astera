/*
 *  Filename: RenderContext.hpp
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

#include "Common/CommonPCH.hpp"
#include "CommandQueue.hpp"

namespace Nth {
    class RenderContext {
    public:
        RenderContext()  = default;
        ~RenderContext() = default;

        N_CLASS_PREVENT_MOVES_COPIES(RenderContext)

        bool Initialize(u32 width, u32 height);
        void Shutdown() const;

        void BeginFrame();
        void EndFrame();

        void Resize(u32 width, u32 height);

        N_ND CommandQueue& GetCommandQueue() {
            return mCommandQueue;
        }

        /// @brief Submit a command to the render queue
        template<typename T>
        void Submit(T&& command) {
            mCommandQueue.Enqueue(std::forward<T>(command));
        }

        N_ND bool GetInitialized() const {
            return mInitialized;
        }

        void GetViewportDimensions(u32& outWidth, u32& outHeight) const {
            outWidth  = mWidth;
            outHeight = mHeight;
        }

    private:
        u32 mWidth {0};
        u32 mHeight {0};
        bool mInitialized {false};

        CommandQueue mCommandQueue;
    };
}  // namespace Nth
