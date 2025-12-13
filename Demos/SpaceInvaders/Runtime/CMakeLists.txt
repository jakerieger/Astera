set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Demos/SpaceInvaders)

add_executable(Demo-SpaceInvaders main.cpp)

target_link_libraries(Demo-SpaceInvaders PRIVATE
    Astera::Astera
)

target_include_directories(Demo-SpaceInvaders PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SOURCE_ROOT}
    ${ENGINE_ROOT}
    ${VENDOR_ROOT}
    ${COMMON_ROOT}
)

add_custom_command(
    TARGET Demo-SpaceInvaders POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/../Content
    $<TARGET_FILE_DIR:Demo-SpaceInvaders>/Content
)

add_custom_command(TARGET Demo-SpaceInvaders POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/EngineContent
    $<TARGET_FILE_DIR:Demo-SpaceInvaders>/Content/EngineContent
)