// Author: Jake Rieger
// Created: 11/27/25.
//

#pragma once

#include "CommonPCH.hpp"

namespace N {
    class Log {
    public:
        static void Initialize();
        static void Shutdown();

        template<typename... Args>
        static void Trace(const string& format, Args&&... args) {
            GetLogger()->trace(fmt::runtime(format), std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Debug(const string& format, Args&&... args) {
            GetLogger()->debug(fmt::runtime(format), std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Info(const string& format, Args&&... args) {
            GetLogger()->info(fmt::runtime(format), std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Warn(const string& format, Args&&... args) {
            GetLogger()->warn(fmt::runtime(format), std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Error(const string& format, Args&&... args) {
            GetLogger()->error(fmt::runtime(format), std::forward<Args>(args)...);
        }

        template<typename... Args>
        static void Critical(const string& format, Args&&... args) {
            GetLogger()->critical(fmt::runtime(format), std::forward<Args>(args)...);
        }

    private:
        static shared_ptr<spdlog::logger> GetLogger();
        static shared_ptr<spdlog::logger> mLogger;
    };
}  // namespace N
