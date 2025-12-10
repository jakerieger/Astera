/*
 *  Filename: CommandQueue.hpp
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
#include "Command.hpp"

namespace Nth {
    /// @brief Command queue for batching and executing rendering commands
    class CommandQueue {
    public:
        CommandQueue()  = default;
        ~CommandQueue() = default;

        N_CLASS_PREVENT_MOVES_COPIES(CommandQueue)

        /// @brief Add a command to the queue
        /// @tparam T Command type (must be one of the types in RenderCommand variant)
        /// @param command The command to enqueue
        template<typename T>
        void Enqueue(T&& command) {
            mCommands.emplace_back(std::forward<T>(command));
        }

        /// @brief Execute all queued commands and clear the queue
        void ExecuteQueue();

        /// @brief Clear all queued commands without executing them
        void Clear();

        /// @brief Get the number of commands in the queue
        N_ND size_t Size() const {
            return mCommands.size();
        }

        /// @brief Check if the queue is empty
        N_ND bool IsEmpty() const {
            return mCommands.empty();
        }

        /// @brief Reserve space for a specific number of commands
        void Reserve(size_t capacity) {
            mCommands.reserve(capacity);
        }

    private:
        /// @brief Execute a single command using visitor pattern
        static void ExecuteCommand(const RenderCommand& command);

        vector<RenderCommand> mCommands;
    };

    /// @brief Command visitor for dispatching commands to their handlers
    class CommandExecutor {
    public:
        void operator()(const ClearCommand& cmd) const;
        void operator()(const DrawSpriteCommand& cmd) const;
        void operator()(const SetViewportCommand& cmd) const;
        void operator()(const BindShaderCommand& cmd) const;
        void operator()(const SetUniformCommand& cmd) const;
    };
}  // namespace N
