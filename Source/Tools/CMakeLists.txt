project(NthEngine)

include(FetchContent)
FetchContent_Declare(
        CLI11
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
        GIT_TAG v2.6.1
)
FetchContent_MakeAvailable(CLI11)

add_subdirectory(NthPak)
add_subdirectory(NthCLI)