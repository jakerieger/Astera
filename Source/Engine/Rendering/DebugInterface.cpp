// Author: Jake Rieger
// Created: 12/9/25.
//

#include "DebugInterface.hpp"

namespace Nth {
    DebugManager::~DebugManager() {
        DetachOverlays();
    }

    void DebugManager::AttachOverlay(IDebugOverlay* overlay) {
        mOverlays.push_back(overlay);
    }

    void DebugManager::DetachOverlays() {
        for (auto& overlay : mOverlays) {
            overlay = nullptr;
        }
        mOverlays.clear();
    }

    void DebugManager::Update(float deltaTime) const {
        for (auto& overlay : mOverlays) {
            overlay->OnUpdate(deltaTime);
        }
    }

    void DebugManager::Render() const {
        for (auto& overlay : mOverlays) {
            overlay->OnRender();
        }
    }

    void DebugManager::HandleEvent(const Event& event) const {
        for (auto& overlay : mOverlays) {
            overlay->OnEvent(event);
        }
    }
}  // namespace Nth