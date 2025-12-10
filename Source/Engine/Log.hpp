/*
 *  Filename: Log.hpp
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

namespace sol {
    class state;
}

namespace Nth {
    class Log {
    public:
        static void Initialize();
        static void Shutdown();

        template<typename... Args>
        static void Trace(const std::string& subsystem, fmt::format_string<Args...> fmt, Args&&... args) {
            GetLogger()->trace("\033[1m{}\033[0m: {}", subsystem, fmt::format(fmt, std::forward<Args>(args)...));
        }

        template<typename... Args>
        static void Debug(const std::string& subsystem, fmt::format_string<Args...> fmt, Args&&... args) {
            GetLogger()->debug("\033[1m{}\033[0m: {}", subsystem, fmt::format(fmt, std::forward<Args>(args)...));
        }

        template<typename... Args>
        static void Info(const std::string& subsystem, fmt::format_string<Args...> fmt, Args&&... args) {
            GetLogger()->info("\033[1m{}\033[0m: {}", subsystem, fmt::format(fmt, std::forward<Args>(args)...));
        }

        template<typename... Args>
        static void Warn(const std::string& subsystem, fmt::format_string<Args...> fmt, Args&&... args) {
            GetLogger()->warn("\033[1m{}\033[0m: {}", subsystem, fmt::format(fmt, std::forward<Args>(args)...));
        }

        template<typename... Args>
        static void Error(const std::string& subsystem, fmt::format_string<Args...> fmt, Args&&... args) {
            GetLogger()->error("\033[1m{}\033[0m: {}", subsystem, fmt::format(fmt, std::forward<Args>(args)...));
        }

        template<typename... Args>
        static void Critical(const std::string& subsystem, fmt::format_string<Args...> fmt, Args&&... args) {
            GetLogger()->critical("\033[1m{}\033[0m: {}", subsystem, fmt::format(fmt, std::forward<Args>(args)...));
        }

    private:
        friend class Game;

        static shared_ptr<spdlog::logger> GetLogger();
        static shared_ptr<spdlog::logger> sLogger;

        static void RegisterLuaGlobals(sol::state& lua);
    };
}  // namespace Nth
