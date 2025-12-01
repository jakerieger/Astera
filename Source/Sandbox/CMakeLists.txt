project(NthEngine)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Sandbox)

add_executable(Sandbox
        main.cpp
)

target_link_libraries(Sandbox PRIVATE NthEngine)

target_include_directories(Sandbox PRIVATE
        ../Engine
        ../Common
)

add_custom_target(CMake_CopyContent ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Content
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Content
        COMMENT "Copying Content directory to build output"
)

add_dependencies(Sandbox CMake_CopyContent)