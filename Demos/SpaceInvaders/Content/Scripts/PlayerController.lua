--- Behavior: PlayerController.lua
--- Created by Astera
---

--- Called at entity initialization
---@param this Entity
function OnAwake(this)
    Log:Debug(string.format("OnAwake() called for '%s'", this.name))
end

--- Called every frame, before rendering
---@param this Entity
---@param clock Clock
function OnUpdate(this, clock)
    this.transform:Translate(ImVec2(100, 0))
end

--- Called every frame, after rendering
---@param this Entity
function OnLateUpdate(this)

end

--- Called at entity destruction
---@param this Entity
function OnDestroyed(this)
    Log:Debug(string.format("OnDestroyed() called for '%s'", this.name))
end
