/*
 *  Filename: AudioEngine.hpp
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

#pragma once

#include "EngineCommon.hpp"

#include "Vendor/miniaudio.h"

namespace sol {
    class state;
}

namespace Astera {
    /// @brief Type alias for sound identifiers
    using SoundID = i32;

    /// @brief Invalid sound ID constant used to indicate errors or uninitialized sounds
    static inline constexpr SoundID kInvalidSoundID {-1};

    /// @brief Manages audio playback and sound resources
    class AudioEngine {
    public:
        AudioEngine();
        ~AudioEngine();

        /// @brief Initializes the audio engine and underlying miniaudio engine
        /// @return True if initialization succeeded, false otherwise
        bool Initialize();

        /// @brief Shuts down the audio engine and releases all resources
        void Shutdown();

        /// @brief Loads a sound file from disk
        /// @param filename Path to the sound file to load
        /// @return Unique sound ID for the loaded sound, or kInvalidSoundID on failure
        SoundID LoadSound(const Path& filename);

        /// @brief Plays a previously loaded sound
        /// @param id The sound ID to play
        /// @param loop Whether the sound should loop continuously (default: false)
        /// @return True if playback started successfully, false otherwise
        bool PlaySound(SoundID id, bool loop = false);

        /// @brief Stops playback of a specific sound
        /// @param id The sound ID to stop
        void StopSound(SoundID id);

        /// @brief Stops playback of all currently playing sounds
        void StopAllSounds() const;

        /// @brief Sets the master volume for all sounds
        /// @param vol Volume level (typically 0.0 to 1.0)
        void SetMasterVolume(f32 vol);

        /// @brief Sets the volume for a specific sound
        /// @param id The sound ID to modify
        /// @param vol Volume level (typically 0.0 to 1.0)
        void SetSoundVolume(SoundID id, f32 vol);

        /// @brief Checks if the audio engine has been successfully initialized
        /// @return True if initialized, false otherwise
        ASTERA_KEEP bool IsInitialized() const {
            return mInitialized;
        }

    private:
        friend class Game;

        /// @brief Internal structure holding sound data and state
        struct Sound {
            /// @brief Miniaudio decoder for the sound file
            ma_decoder decoder;
            /// @brief Miniaudio sound object for playback
            ma_sound sound;
            /// @brief Path to the sound file on disk
            string filepath;
            /// @brief Whether the sound has been successfully loaded
            bool loaded = false;
        };

        /// @brief Whether the audio engine is initialized
        bool mInitialized {false};
        /// @brief Underlying miniaudio engine instance
        ma_engine mEngine {};
        /// @brief Next available sound ID to assign
        SoundID mNextSoundId {1};
        /// @brief Map of sound IDs to their corresponding sound data
        std::unordered_map<SoundID, unique_ptr<Sound>> mSounds;

        /// @brief Retrieves a sound object by its ID
        /// @param id The sound ID to retrieve
        /// @return Pointer to the sound object, or nullptr if not found
        Sound* GetSound(SoundID id);

        /// @brief Registers audio-related functions with the Lua scripting environment
        /// @param lua Reference to the Lua state
        void RegisterLuaGlobals(sol::state& lua);
    };
}  // namespace Astera