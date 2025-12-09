--- Behavior: BallController.lua
--- Created by Nth Engine
---

ballSpeed = 500
bgMusicID = 1

---@param this Entity
function OnAwake(this)
    Log:Debug(string.format("OnAwake() called for entity: %s", this.name))

    if AudioPlayer:IsInitialized() then
        AudioPlayer:SetSoundVolume(bgMusicID, 0.5)
        AudioPlayer:PlaySound(bgMusicID, true)
    end
end

---Handle movement of ball sprite based on input
---@param transform Transform
---@param dT number
function HandleMovement(transform, dT)
    local newPosition = Vec2(0, 0)

    -- Check is one of our movement keys is pressed
    if Input:GetKeyDown(KeyCode.W) then
        -- Apply a new value to the correct axis scaled by delta time
        newPosition.y = (ballSpeed * dT)
    end
    if Input:GetKeyDown(KeyCode.S) then
        newPosition.y = -(ballSpeed * dT)
    end
    if Input:GetKeyDown(KeyCode.A) then
        newPosition.x = -(ballSpeed * dT)
    end
    if Input:GetKeyDown(KeyCode.D) then
        newPosition.x = (ballSpeed * dT)
    end

    -- Translate our entity by the corresponding new amount
    transform:Translate(newPosition)
end

---@param this Entity
---@param clock Clock
function OnUpdate(this, clock)
    HandleMovement(this.transform, clock:GetDeltaTime())
end

---@param this Entity
function OnDestroyed(this)
    Log:Debug(string.format("OnDestroyed() called for entity: %s", this.name))
end
