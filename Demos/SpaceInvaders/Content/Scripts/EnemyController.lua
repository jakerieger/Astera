--- Behavior: EnemyController.lua
--- Created by Astera
---

---Called at entity initialization
---@param this Entity
function OnAwake(this)
    Log:Debug("Test message from EnemyController.lua")
end

local speed = 200
local deltaX = speed

--- Check if current position is within specified bounds
---@param current number
---@param leftBound number
---@param rightBound number
---@return boolean
function CheckOOB(current, leftBound, rightBound)
    return (current < leftBound) or (current > rightBound)
end

---Called every frame, before rendering
---@param this Entity
---@param clock Clock
function OnUpdate(this, clock)
    local transform = this.transform
    local xBounds = 200
    local centerX = Game:GetScreenSize().x / 2
    local currentPosX = transform.position.x

    if CheckOOB(currentPosX, (centerX - xBounds), (centerX + xBounds)) then
        deltaX = -deltaX
    end

    transform:Translate(Vec2(deltaX * clock:GetDeltaTime(), 0))
end

---Called every frame, after rendering
---@param this Entity
function OnLateUpdate(this)

end

---Called at entity destruction
---@param this Entity
function OnDestroyed(this)

end
