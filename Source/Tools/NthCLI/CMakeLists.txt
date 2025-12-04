project(NthEngine)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Tools/NthCLI)

add_executable(nth
        ProjectRunner.hpp
        ProjectRunner.cpp
        main.cpp
)

target_link_libraries(nth PRIVATE
        CLI11
        fmt::fmt
        NthEngine
)

target_include_directories(nth PRIVATE
        ../../../Source/Common
        ../../../Source/Engine
)