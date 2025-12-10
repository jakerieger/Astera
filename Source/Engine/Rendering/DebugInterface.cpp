// Author: Jake Rieger
// Created: 12/9/25.
//

#include "DebugInterface.hpp"
#include <ranges>

namespace Nth {
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
}  // namespace Nth