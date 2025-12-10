/// @author Jake Rieger
/// @created 11/27/25
///

#pragma once

#include "Common/CommonPCH.hpp"

#ifdef N_ENGINE_PLATFORM_WINDOWS
    #include <windows.h>
#else
    #include <ctime>
#endif

namespace Nth {
    /// @brief High-precision clock for timing and frame rate tracking
    ///
    /// Provides accurate time measurements for delta time calculation, frame rate monitoring,
    /// and total elapsed time tracking. Uses platform-specific high-resolution timers
    /// (QueryPerformanceCounter on Windows, clock_gettime on other platforms).
    class Clock {
    public:
        /// @brief Constructs and initializes the clock
        ///
        /// Queries the system's high-resolution timer frequency and sets the initial time.
        Clock();

        /// @brief Updates the clock and calculates time deltas
        ///
        /// Should be called once per frame to update delta time, total time, and FPS.
        /// Automatically tracks frame count and updates FPS at regular intervals.
        void Tick();

        /// @brief Gets the precise time elapsed since the last tick
        /// @return Delta time in seconds as a double-precision value
        N_ND f64 GetDeltaTimePrecise() const;

        /// @brief Gets the time elapsed since the last tick
        /// @return Delta time in seconds as a single-precision value
        N_ND f32 GetDeltaTime() const;

        /// @brief Gets the total time elapsed since clock creation
        /// @return Total elapsed time in seconds
        N_ND f64 GetTotalTime() const;

        /// @brief Gets the current frames per second
        /// @return Current FPS (updated at regular intervals, not every frame)
        N_ND f64 GetFramesPerSecond() const;

        /// @brief Gets the raw high-resolution counter value
        /// @return Current counter value in platform-specific ticks
        N_ND u64 GetRawCounter() const;

        /// @brief Gets the frequency of the high-resolution counter
        /// @return Counter frequency in ticks per second
        N_ND u64 GetCounterFrequency() const;

    private:
#ifdef N_ENGINE_PLATFORM_WINDOWS
        /// @brief High-resolution counter frequency (Windows)
        LARGE_INTEGER mFrequency;

        /// @brief Last recorded counter value (Windows)
        LARGE_INTEGER mLastTime;
#else
        /// @brief High-resolution counter frequency (non-Windows)
        u64 mFrequency;

        /// @brief Last recorded counter value in nanoseconds (non-Windows)
        u64 mLastTime;
#endif

        /// @brief Time elapsed since last tick in seconds
        f64 mDeltaTime;

        /// @brief Total time elapsed since clock creation in seconds
        f64 mTotalTime;

        /// @brief Total number of frames (ticks) since clock creation
        u64 mFrameCount;

        /// @brief Current frames per second measurement
        f64 mFramesPerSecond;

        /// @brief Interval at which FPS is recalculated in seconds
        f64 mFpsUpdateInterval;

        /// @brief Time accumulated since last FPS update in seconds
        f64 mTimeSinceLastFpsUpdate;
    };
}  // namespace Nth