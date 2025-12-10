/*
 *  Filename: Geometry.hpp
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
#include "VertexArray.hpp"

namespace Nth {
    /// @brief Vertex structure for sprite/quad rendering (position + texcoord packed)
    struct SpriteVertex {
        f32 x, y;  // Position
        f32 u, v;  // Texture coordinates

        SpriteVertex() = default;
        SpriteVertex(f32 x, f32 y, f32 u, f32 v) : x(x), y(y), u(u), v(v) {}
    };

    using GeometryHandle = shared_ptr<class Geometry>;

    /// @brief High-level geometry abstraction that manages vertex/index data and VAO setup
    class Geometry {
    public:
        Geometry()  = default;
        ~Geometry() = default;

        N_CLASS_PREVENT_MOVES_COPIES(Geometry)

        /// @brief Create a quad geometry for sprite rendering
        /// @param width Width of the quad
        /// @param height Height of the quad
        /// @return Shared pointer to the created geometry
        static GeometryHandle CreateQuad(f32 width = 1.0f, f32 height = 1.0f);

        /// @brief Bind this geometry for rendering
        void Bind() const;

        void Destroy() const;

        /// @brief Unbind the currently bound geometry
        static void Unbind();

        N_ND const shared_ptr<VertexArray>& GetVertexArray() const {
            return mVertexArray;
        }

        /// @brief Get the number of indices in this geometry
        /// @return Index count
        N_ND u32 GetIndexCount() const {
            N_ASSERT(mVertexArray != nullptr);
            N_ASSERT(mVertexArray->GetIndexBuffer() != nullptr);
            return CAST<u32>(mVertexArray->GetIndexBuffer()->GetCount());
        }

    private:
        shared_ptr<VertexArray> mVertexArray;
    };
}  // namespace Nth