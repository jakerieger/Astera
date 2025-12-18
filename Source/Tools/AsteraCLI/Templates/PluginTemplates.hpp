#pragma once

#include <string_view>
#include <string>
#include <Shared/MacroExpander.hpp>

namespace AsteraCLI::PluginTemplates {
    inline constexpr std::string_view CMakeLists_TXT = R"(cmake_minimum_required(VERSION 3.14)
project(${PluginName})

add_library(${PluginName} SHARED
    PluginExport.hpp
    ${PluginName}.cpp
    ${PluginName}.hpp

target_compile_definitions(${PluginName} PRIVATE ${PluginExports})
)";

    inline constexpr std::string_view PluginExport_HPP = R"(#pragma once

#ifdef _WIN32
    #ifdef ${PluginExports}
        #define ${PluginAPI} __declspec(dllexport)
    #else
        #define ${PluginAPI} __declspec(dllimport)
    #endif
#else
    #if __GNUC__ >= 4
        #define ${PluginAPI} __attribute__((visibility("default")))
    #else
        #define ${PluginAPI}
    #endif
#endif)";

    inline constexpr std::string_view Plugin_HPP = R"(#pragma once

#include <AsteraCore.hpp>
#include <EnginePluginInterface.hpp>
#include "PluginExport.hpp"

namespace Astera {
    class ${PluginAPI} ${PluginName} final : public IEnginePlugin {
    public:
        ${PluginName}() {
            this->mName = "${PluginName}";
        }

        void OnEngineStart(Game* engine) override;
        void OnEngineStop(Game* engine) override;
    };
}  // namespace Astera

extern "C" ${PluginAPI} Astera::IEnginePlugin* CreatePlugin();)";

    inline constexpr std::string_view Plugin_CPP = R"(#include "${PluginName}.hpp"

#include <cstdio>

namespace Astera {
    void ${PluginName}::OnEngineStart(Game* engine) {
        Log::Warn(mName, "OnEngineStart() called with engine instance: {}", fmt::ptr(engine));
    }

    void ${PluginName}::OnEngineStop(Game* engine) {
        Log::Warn(mName, "OnEngineStop() called with engine instance: {}", fmt::ptr(engine));
    }
}  // namespace Astera

extern "C" ${PluginAPI} Astera::IEnginePlugin* CreatePlugin() {
    return new Astera::SamplePlugin();
})";

    inline std::string Make_CMakeLists_TXT(const std::string_view pluginName, const std::string_view pluginExports) {
        auto output = std::string(CMakeLists_TXT.begin(), CMakeLists_TXT.end());

        const std::unordered_map<std::string, std::string> vars = {{"PluginName", pluginName.data()},
                                                                   {"PluginExports", pluginExports.data()}};
        Astera::ExpandMacros(output, vars);

        return output;
    }

    inline std::string Make_PluginExport_HPP(const std::string_view pluginExports, std::string_view pluginAPI) {
        auto output = std::string(PluginExport_HPP.begin(), PluginExport_HPP.end());

        const std::unordered_map<std::string, std::string> vars = {{"PluginExports", pluginExports.data()},
                                                                   {"PluginAPI", pluginAPI.data()}};
        Astera::ExpandMacros(output, vars);

        return output;
    }

    inline std::string Make_Plugin_HPP(const std::string_view pluginName, const std::string_view pluginAPI) {
        auto output = std::string(Plugin_HPP.begin(), Plugin_HPP.end());

        const std::unordered_map<std::string, std::string> vars = {{"PluginName", pluginName.data()},
                                                                   {"PluginAPI", pluginAPI.data()}};
        Astera::ExpandMacros(output, vars);

        return output;
    }

    inline std::string Make_Plugin_CPP(const std::string_view pluginName, const std::string_view pluginAPI) {
        auto output = std::string(Plugin_CPP.begin(), Plugin_CPP.end());

        const std::unordered_map<std::string, std::string> vars = {{"PluginName", pluginName.data()},
                                                                   {"PluginAPI", pluginAPI.data()}};
        Astera::ExpandMacros(output, vars);

        return output;
    }
}  // namespace AsteraCLI::PluginTemplates