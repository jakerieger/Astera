/*
 *  Filename: IO.cpp
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

#include "IO.hpp"
#include <fstream>

namespace Astera {
    Result<vector<u8>> IO::ReadBytes(const Path& filename) {
        if (!exists(filename)) {
            return unexpected(fmt::format("File does not exist: {}", filename.string()));
        }

        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            return unexpected(fmt::format("Failed to open file: {}", filename.string()));
        }

        const std::streamsize fileSize = file.tellg();
        vector<u8> bytes(fileSize);
        file.seekg(0, std::ios::beg);
        if (!file.read(RCAST<char*>(bytes.data()), fileSize)) {
            return unexpected("Failed to read file");
        }
        file.close();

        return bytes;
    }

    Result<string> IO::ReadText(const Path& filename) {
        if (!exists(filename)) {
            return unexpected(fmt::format("File does not exist: {}", filename.string()));
        }
        const std::ifstream file(filename);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
}  // namespace Astera