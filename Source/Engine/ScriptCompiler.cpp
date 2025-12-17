/*
 *  Filename: ScriptCompiler.cpp
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

#include "ScriptCompiler.hpp"
#include "StringConvert.inl"

extern "C" {
#include <lualib.h>
#include <lauxlib.h>
#include <luajit.h>
}

namespace Astera {
    static constexpr i32 kLuaKeepDebugInfo {0};
    static constexpr i32 kLuaStripDebugInfo {1};

    Result<vector<u8>> ScriptCompiler::Compile(const string& source, const string& chunkName) {
        vector<u8> bytecode;

        lua_State* L = luaL_newstate();
        if (!L) {
            return unexpected("Failed to create Lua state");
        }

        const i32 loadResult = luaL_loadbuffer(L, source.c_str(), source.size(), chunkName.c_str());
        if (loadResult != 0) {
            lua_close(L);
            return unexpected(fmt::format("Failed to compile Lua script: {}", lua_tostring(L, -1)));
        }

        BytecodeWriterState writerState;
        writerState.bytecode = &bytecode;

// Visual Studio 2022 uses Lua 5.2 for some reason, this fixes the discrepancy
// TODO: Enforce the same Lua version between IDE project files
#if LUA_VERSION_NUM >= 502
        const i32 dumpResult = lua_dump(L, BytecodeWriter, &writerState, kLuaKeepDebugInfo);
#else
        const i32 dumpResult = lua_dump(L, BytecodeWriter, &writerState);
#endif

        if (dumpResult != 0) {
            bytecode.clear();
            return unexpected("Failed to dump Lua bytecode: " + StringConvert::ToString(dumpResult));
        }

        lua_close(L);
        return bytecode;
    }

    i32 ScriptCompiler::BytecodeWriter(lua_State* L, const void* p, size_t size, void* ud) {
        const BytecodeWriterState* state = CAST<BytecodeWriterState*>(ud);
        const auto data                  = CAST<const u8*>(p);
        state->bytecode->insert(state->bytecode->end(), data, data + size);
        return 0;
    }
}  // namespace Astera