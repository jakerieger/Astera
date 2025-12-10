/*
 *  Filename: Clock.cpp
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

#include "Clock.hpp"

namespace Astera {
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