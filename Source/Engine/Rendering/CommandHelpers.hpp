/*
 *  Filename: CommandHelpers.hpp
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

#include "Command.hpp"

namespace Nth {
    /// @brief Helper functions for creating buffer update commands
    namespace CommandHelpers {

        /// @brief Create a vertex buffer update command from typed data
        /// @tparam T Vertex type
        /// @param buffer Buffer to update
        /// @param vertices Vector of vertex data
        /// @param offset Offset in bytes
        /// @return UpdateVertexBufferCommand ready to submit
        template<typename T>
        UpdateVertexBufferCommand
        CreateVertexBufferUpdate(const shared_ptr<VertexBuffer>& buffer, const vector<T>& vertices, size_t offset = 0) {
            vector<u8> data(vertices.size() * sizeof(T));
            std::memcpy(data.data(), vertices.data(), data.size());

            return UpdateVertexBufferCommand {.buffer = buffer, .data = std::move(data), .offset = offset};
        }

        /// @brief Create an index buffer update command
        /// @param buffer Buffer to update
        /// @param indices Vector of indices
        /// @param offset Offset in number of indices (not bytes)
        /// @return UpdateIndexBufferCommand ready to submit
        inline UpdateIndexBufferCommand
        CreateIndexBufferUpdate(const shared_ptr<IndexBuffer>& buffer, const vector<u32>& indices, size_t offset = 0) {
            return UpdateIndexBufferCommand {.buffer = buffer, .indices = indices, .offset = offset};
        }
    }  // namespace CommandHelpers
}  // namespace Nth