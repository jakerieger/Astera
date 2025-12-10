set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Sandbox)
add_executable(Sandbox main.cpp)

target_link_libraries(Sandbox PRIVATE
    Astera::Astera
)

add_custom_target(CMake_CopySandboxContent ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Sandbox/Content
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Content
    COMMENT "Copying Sandbox content directory to build output"
)

add_custom_target(CMake_CopyEngineContent ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/EngineContent
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Content/EngineContent
    COMMENT "Copying Engine content directory to build output"
)

add_custom_target(CMake_CopySandboxResources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/Res
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Res
    COMMENT "Copying Sandbox resources directory to build output"
)

# The final hierarchy should be GamePath/Content/EngineContent
add_dependencies(CMake_CopyEngineContent CMake_CopySandboxContent)
add_dependencies(CMake_CopySandboxResources CMake_CopyEngineContent)
add_dependencies(Sandbox CMake_CopySandboxResources)