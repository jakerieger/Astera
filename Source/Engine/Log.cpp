/*
 *  Filename: Log.cpp
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

#include "Log.hpp"

#include <sol/sol.hpp>

namespace Astera {
    shared_ptr<spdlog::logger> Log::sLogger;

    void Log::Initialize() {
#ifdef _WIN32
        // Enable ANSI escape codes on Windows 10+
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut != INVALID_HANDLE_VALUE) {
            DWORD dwMode = 0;
            if (GetConsoleMode(hOut, &dwMode)) {
                dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                SetConsoleMode(hOut, dwMode);
            }
        }
#endif

        // Create a color console sink
        auto consoleSink = make_shared<spdlog::sinks::stdout_color_sink_mt>();

        // Set the pattern: [HH:MM:SS] [level] message
        consoleSink->set_pattern("[%T] [%^%l%$] %v");

        // Create logger with the sink
        sLogger = std::make_shared<spdlog::logger>("main", consoleSink);
#ifndef NDEBUG
        sLogger->set_level(spdlog::level::trace);
        sLogger->flush_on(spdlog::level::trace);
#else
        sLogger->set_level(spdlog::level::info);
        sLogger->flush_on(spdlog::level::info);
#endif

        spdlog::register_logger(sLogger);
        spdlog::set_default_logger(sLogger);

        Info("Log", "Logging system initialized");
    }

    void Log::Shutdown() {
        if (sLogger) {
            Info("Log", "Shutting down logging system");
            sLogger->flush();
            spdlog::drop_all();
            sLogger.reset();
        }
    }

    shared_ptr<spdlog::logger> Log::GetLogger() {
        if (!sLogger) { Initialize(); }
        return sLogger;
    }

    void Log::RegisterLuaGlobals(sol::state& lua) {
        lua["Log"] = lua.create_table();

        lua["Log"]["Debug"] = [](sol::object owningTable, sol::object msg) {
            Log::GetLogger()->debug("\033[1m{}\033[0m: {}", "LuaInterpreter", msg.as<string>());
        };
        lua["Log"]["Info"] = [](sol::object owningTable, sol::object msg) {
            Log::GetLogger()->info("\033[1m{}\033[0m: {}", "LuaInterpreter", msg.as<string>());
        };
        lua["Log"]["Warn"] = [](sol::object owningTable, sol::object msg) {
            Log::GetLogger()->warn("\033[1m{}\033[0m: {}", "LuaInterpreter", msg.as<string>());
        };
        lua["Log"]["Error"] = [](sol::object owningTable, sol::object msg) {
            Log::GetLogger()->error("\033[1m{}\033[0m: {}", "LuaInterpreter", msg.as<string>());
        };
        lua["Log"]["Critical"] = [](sol::object owningTable, sol::object msg) {
            Log::GetLogger()->critical("\033[1m{}\033[0m: {}", "LuaInterpreter", msg.as<string>());
        };
    }
}  // namespace Astera