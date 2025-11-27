// Author: Jake Rieger
// Created: 11/27/25.
//

#include "Log.hpp"

namespace N {
    shared_ptr<spdlog::logger> Log::mLogger;

    void Log::Initialize() {
        // Create console sink with color support
        auto consoleSink = make_shared<spdlog::sinks::stdout_color_sink_mt>();
        consoleSink->set_pattern("[%T] [%^%l%$] %n: %v");

        // Create logger
        mLogger = make_shared<spdlog::logger>("Engine", consoleSink);
        mLogger->set_level(spdlog::level::trace);
        mLogger->flush_on(spdlog::level::trace);

        // Register as default logger
        spdlog::register_logger(mLogger);
        spdlog::set_default_logger(mLogger);

        Info("Logging system initialized");
    }

    void Log::Shutdown() {
        if (mLogger) {
            Info("Shutting down logging system");
            mLogger->flush();
            spdlog::drop_all();
            mLogger.reset();
        }
    }

    shared_ptr<spdlog::logger> Log::GetLogger() {
        if (!mLogger) { Initialize(); }
        return mLogger;
    }
}  // namespace N