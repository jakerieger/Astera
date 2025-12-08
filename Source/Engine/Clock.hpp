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
    class Clock {
    public:
        Clock();

        void Tick();

        N_ND f64 GetDeltaTimePrecise() const;
        N_ND f32 GetDeltaTime() const;
        N_ND f64 GetTotalTime() const;
        N_ND f64 GetFramesPerSecond() const;
        N_ND u64 GetRawCounter() const;
        N_ND u64 GetCounterFrequency() const;

    private:
#ifdef N_ENGINE_PLATFORM_WINDOWS
        LARGE_INTEGER mFrequency;
        LARGE_INTEGER mLastTime;
#else
        u64 mFrequency;
        u64 mLastTime;
#endif

        f64 mDeltaTime;
        f64 mTotalTime;
        u64 mFrameCount;
        f64 mFramesPerSecond;
        f64 mFpsUpdateInterval;
        f64 mTimeSinceLastFpsUpdate;
    };
}  // namespace N
