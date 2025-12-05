/// @author: Jake Rieger
/// @created: 12/3/2025.
///

#include <string>
#include <filesystem>

#include <CLI/CLI.hpp>
#include <fmt/core.h>
#include <fmt/color.h>

#include "Content.hpp"
#include "ProjectDescriptor.hpp"
#include "ProjectRunner.hpp"

namespace fs = std::filesystem;

/* Project commands:
 * create <name> // Creates a new project
 * info <path> // Prints info on the given project
 * run <path> // Runs the project
 *
 * Example:
 * nth project create BallRoller
 */

namespace ProjectSubcommand {
    static std::string value;

    static void Create() {
        // Create project directory at specified location
        const auto projectDir = fs::path(value);
        if (!fs::exists(projectDir)) { fs::create_directories(projectDir); }

        // Create Content/Runtime directories
        const auto contentDir = projectDir / "Content";
        if (!fs::exists(contentDir)) { fs::create_directories(contentDir); }

        const auto runtimeDir = projectDir / "Runtime";
        if (!fs::exists(runtimeDir)) { fs::create_directories(runtimeDir); }

        const auto projectFile = projectDir / fmt::format("{}.nthproj", value);
        Nth::ProjectDescriptor projectDescriptor;
        projectDescriptor.name              = value;
        projectDescriptor.contentPath       = "${ProjectRoot}/Content";
        projectDescriptor.engineContentPath = "${ProjectRoot}/EngineContent";
        projectDescriptor.engineVersion     = 1;
        projectDescriptor.startupScene      = "Example";

        const auto doc = projectDescriptor.Serialize();
        if (!doc.save_file(projectFile.string().c_str())) { throw std::runtime_error("Failed to create project file"); }

        // TODO: Create runtime CMake project
    }

    static void Info() {}

    static void Run() {
        using namespace Nth;
        ProjectDescriptor descriptor;
        ProjectDescriptor::Deserialize(value, descriptor);

        Content::SetContentPath(descriptor.contentPath);
        Content::SetEngineContentPath(descriptor.engineContentPath);

        ProjectRunner projectRunner(descriptor.name);
        projectRunner.SetStartupScene(descriptor.startupScene);
        projectRunner.Run();
    }
}  // namespace ProjectSubcommand

int main(int argc, char* argv[]) {
    CLI::App app {"nth - Project management tool"};
    app.require_subcommand(1);

    CLI::App* project = app.add_subcommand("project", "Manage projects");
    project->require_subcommand(1);

    CLI::App* create = project->add_subcommand("create", "Creates a new project in the current directory");
    create->add_option("name", ProjectSubcommand::value, "Project name")->required();
    create->callback([&]() { ProjectSubcommand::Create(); });

    CLI::App* info = project->add_subcommand("info", "Prints info on the given project");
    info->add_option("path", ProjectSubcommand::value, "Project path")->required();
    info->callback([&]() { ProjectSubcommand::Info(); });

    CLI::App* run = project->add_subcommand("run", "Runs the project");
    run->add_option("path", ProjectSubcommand::value, "Project path")->required();
    run->callback([&]() { ProjectSubcommand::Run(); });

    CLI11_PARSE(app, argc, argv);
}