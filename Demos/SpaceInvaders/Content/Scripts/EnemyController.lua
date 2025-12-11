--- Behavior: EnemyController.lua
--- Created by Astera
---

---Called at entity initialization
---@param this Entity
function OnAwake(this)

end

---Called every frame, before rendering
---@param this Entity
---@param clock Clock
function OnUpdate(this, clock)
    local player = Scene:FindEntityByName("Player")
    if player ~= InvalidEntityID then
        -- Get player location
        local playerTransform = Scene:GetEntityTransform(player)
        -- Rotate to face player
    end
end

---Called every frame, after rendering
---@param this Entity
function OnLateUpdate(this)

end

---Called at entity destruction
---@param this Entity
function OnDestroyed(this)

end
