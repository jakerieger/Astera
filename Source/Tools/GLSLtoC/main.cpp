/*
 * This program's entire purpose is to convert the GLSL source files in Source/Engine/Shaders/GLSL into C header files
 * that can be included in the engine source code. This effectively embeds the shader code within the engine binaries
 * and removes the need to ship shader sources with the engine.
 *
 * Not intended for external use.
 */

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <filesystem>
#include <sstream>
#include <fstream>

int main(int argc, char* argv[]) {
    using namespace rapidjson;
    using namespace std;
    using namespace filesystem;

    if (argc != 2) {
        fprintf(stderr, "Manifest file not supplied");
        return EXIT_FAILURE;
    }

    // read in manifest.json file from shaders directory
    const auto manifestPath = path(argv[1]);
    if (!exists(manifestPath)) {
        fprintf(stderr, "Manifest file does not exist");
        return EXIT_FAILURE;
    }

    auto ReadFile = [](const path& filename) -> string {
        const ifstream file(filename);
        stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    };

    // read manifest contents
    Document manifest;
    manifest.Parse(ReadFile(manifestPath).c_str());

    assert(manifest.HasMember("shaders"));
    if (manifest["shaders"].HasMember("rendering")) {
        auto rendering = manifest["shaders"]["rendering"].GetArray();
        for (auto it = rendering.Begin(); it != rendering.End(); ++it) {
            assert(it->IsObject());
            assert(it->HasMember("name"));
            assert(it->HasMember("vertex"));
            assert(it->HasMember("fragment"));

            const auto name     = it->GetObject()["name"].GetString();
            const auto vertex   = it->GetObject()["vertex"].GetString();
            const auto fragment = it->GetObject()["fragment"].GetString();

            printf("Found shader '%s':\n-- %s\n-- %s\n", name, vertex, fragment);

            // Read file contents of each shader and create the string objects to write to the header file
            // TODO: Strip comments
            const auto vertexSource   = ReadFile(manifestPath.parent_path() / vertex);
            const auto fragmentSource = ReadFile(manifestPath.parent_path() / fragment);

            const auto headerName = string(name) + ".inc";
            const auto headerPath = manifestPath.parent_path() / "Include" / headerName;

            stringstream headerBuffer;
            headerBuffer << "// Created with GLSLtoC\n";
            headerBuffer << "// Shader program: " << name << "\n\n#pragma once\n\n";
            headerBuffer << "inline static const char* k" << name << "Vertex = R\"\"(" << vertexSource << ")\"\";\n\n";
            headerBuffer << "inline static const char* k" << name << "Fragment = R\"\"(" << fragmentSource
                         << ")\"\";\n\n";

            // write header file to disk
            ofstream headerOut(headerPath);
            headerOut << headerBuffer.str();
            headerOut.close();
        }
    }
    // TODO: Implement the code for compute shaders
    // if (manifest["shaders"].HasMember("compute")) {}

    printf("Finished generating shader includes.\n");
}