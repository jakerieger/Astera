/*
 *  Filename: Geometry.cpp
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

#include "Geometry.hpp"
#include "GLUtils.hpp"
#include "Log.hpp"

namespace Nth {
    GeometryHandle Geometry::CreateQuad(f32 width, f32 height) {
        auto geometry = make_shared<Geometry>();

        // Calculate half-extents for centered quad
        const f32 halfWidth  = width * 0.5f;
        const f32 halfHeight = height * 0.5f;

        // Define quad vertices (position + texcoord packed as vec4)
        // Triangle strip ordering: bottom-left, bottom-right, top-left, top-right
        const SpriteVertex vertices[] = {
          {-halfWidth, -halfHeight, 0.0f, 0.0f},  // Bottom-left
          {halfWidth, -halfHeight, 1.0f, 0.0f},   // Bottom-right
          {-halfWidth, halfHeight, 0.0f, 1.0f},   // Top-left
          {halfWidth, halfHeight, 1.0f, 1.0f},    // Top-right
        };

        // Define indices for two triangles
        const u32 indices[] = {
          0,
          1,
          2,  // First triangle
          2,
          1,
          3  // Second triangle
        };

        // Create vertex buffer and upload data
        auto vertexBuffer = make_shared<VertexBuffer>();
        vertexBuffer->SetData(vertices, sizeof(vertices), BufferUsage::Static);

        // Create index buffer and upload data
        auto indexBuffer = make_shared<IndexBuffer>();
        indexBuffer->SetIndices(indices, 6, BufferUsage::Static);

        // Define vertex layout matching the shader's input
        // layout (location = 0) in vec4 aVertex;
        VertexLayout layout;
        layout.AddAttribute(VertexAttribute("aVertex", AttributeType::Float4));

        // Create and configure VAO
        geometry->mVertexArray = make_shared<VertexArray>();
        geometry->mVertexArray->AddVertexBuffer(vertexBuffer, layout);
        geometry->mVertexArray->SetIndexBuffer(indexBuffer);

        Log::Debug("Geometry", "Created quad geometry ({}x{})", width, height);

        return geometry;
    }

    void Geometry::Bind() const {
        if (mVertexArray) { mVertexArray->Bind(); }
    }

    void Geometry::Destroy() const {
        mVertexArray->Destroy();
    }

    void Geometry::Unbind() {
        VertexArray::Unbind();
    }
}  // namespace Nth