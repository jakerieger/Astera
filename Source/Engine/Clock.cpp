/// @author Jake Rieger
/// @created 11/27/25
///
#include "Clock.hpp"

namespace Nth {
    Clock::Clock() {
#ifdef N_ENGINE_PLATFORM_WINDOWS
        QueryPerformanceFrequency(&mFrequency);
        QueryPerformanceCounter(&mLastTime);
#else
        // Linux uses clock_gettime with CLOCK_MONOTONIC
        mFrequency = 1000000000ULL;  // nanoseconds per second

        struct timespec ts {};
        clock_gettime(CLOCK_MONOTONIC, &ts);
        mLastTime = CAST<u64>(ts.tv_sec) * 1000000000ULL + CAST<u64>(ts.tv_nsec);
#endif

        mDeltaTime              = 0.0;
        mTotalTime              = 0.0;
        mFrameCount             = 0;
        mFramesPerSecond        = 0.0;
        mFpsUpdateInterval      = 0.5;  // In seconds
        mTimeSinceLastFpsUpdate = 0.0;
    }

    void Clock::Tick() {
#ifdef N_ENGINE_PLATFORM_WINDOWS
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);

        mDeltaTime = CAST<f64>(currentTime.QuadPart - mLastTime.QuadPart) / CAST<f64>(mFrequency.QuadPart);
        mLastTime  = currentTime;
#else
        struct timespec ts {};
        clock_gettime(CLOCK_MONOTONIC, &ts);
        u64 currentTime = CAST<u64>(ts.tv_sec) * 1000000000ULL + CAST<u64>(ts.tv_nsec);

        mDeltaTime = CAST<f64>(currentTime - mLastTime) / CAST<f64>(mFrequency);
        mLastTime  = currentTime;
#endif

        mTotalTime += mDeltaTime;
        mFrameCount++;
        mTimeSinceLastFpsUpdate += mDeltaTime;

        if (mTimeSinceLastFpsUpdate >= mFpsUpdateInterval) {
            mFramesPerSecond        = CAST<f64>(mFrameCount) / mTimeSinceLastFpsUpdate;
            mFrameCount             = 0;
            mTimeSinceLastFpsUpdate = 0.0;
        }
    }

    f64 Clock::GetDeltaTimePrecise() const {
        return mDeltaTime;
    }

    f32 Clock::GetDeltaTime() const {
        return CAST<f32>(mDeltaTime);
    }

    f64 Clock::GetTotalTime() const {
        return mTotalTime;
    }

    f64 Clock::GetFramesPerSecond() const {
        return mFramesPerSecond;
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    u64 Clock::GetRawCounter() const {  // NOLINT(*-convert-member-functions-to-static)
#ifdef N_ENGINE_PLATFORM_WINDOWS
        LARGE_INTEGER current;
        QueryPerformanceCounter(&current);
        return current.QuadPart;
#else
        struct timespec ts {};
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return CAST<u64>(ts.tv_sec) * 1000000000ULL + CAST<u64>(ts.tv_nsec);
#endif
    }

    u64 Clock::GetCounterFrequency() const {
#ifdef N_ENGINE_PLATFORM_WINDOWS
        return mFrequency.QuadPart;
#else
        return mFrequency;
#endif
    }
}  // namespace N