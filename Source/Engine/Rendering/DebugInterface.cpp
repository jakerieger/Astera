/*
 *  Filename: DebugInterface.cpp
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

#include "DebugInterface.hpp"
#include <ranges>

namespace Astera {
    DebugManager::~DebugManager() {
        DetachOverlays();
    }

    void DebugManager::AttachOverlay(const std::string& name, IDebugOverlay* overlay) {
        mOverlays.insert_or_assign(name, Overlay {overlay, true});
    }

    void DebugManager::DetachOverlays() {
        for (auto [name, overlay] : mOverlays) {
            overlay.overlay = nullptr;
            overlay.enabled = false;
        }
        mOverlays.clear();
    }

    void DebugManager::SetOverlayEnabled(const std::string& name, bool enabled) {
        const auto it = mOverlays.find(name);
        if (it != mOverlays.end()) { it->second.enabled = enabled; }
    }

    bool DebugManager::GetOverlayEnabled(const std::string& name) const {
        return mOverlays.at(name).enabled;
    }

    void DebugManager::Update(float deltaTime) const {
        for (const auto overlay : mOverlays | std::views::values) {
            overlay.overlay->OnUpdate(deltaTime);
        }
    }

    void DebugManager::Render() const {
        for (const auto overlay : mOverlays | std::views::values) {
            if (overlay.enabled) { overlay.overlay->OnRender(); }
        }
    }

    void DebugManager::HandleEvent(const Event& event) const {
        for (const auto overlay : mOverlays | std::views::values) {
            overlay.overlay->OnEvent(event);
        }
    }
}  // namespace Astera