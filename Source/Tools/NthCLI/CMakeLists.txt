project(NthCLI)

file(GLOB CLI_SOURCES
        *.cpp
        *.hpp
)

add_executable(nth ${CLI_SOURCES})

set_target_properties(nth PROPERTIES
        SKIP_PRECOMPILE_HEADERS ON
)

target_link_libraries(nth PRIVATE
        ${NTHENGINE_LIBS}
        CLI11
        NthEngine
)

target_include_directories(nth PRIVATE
        ${CMAKE_SOURCE_DIR}/Source
)

# Set output name based on platform
if (WIN32)
    set_target_properties(nth PROPERTIES OUTPUT_NAME "nth")
else ()
    set_target_properties(nth PROPERTIES OUTPUT_NAME "nth")
endif ()

# ============================================================================
# Installation Configuration
# ============================================================================

# Install the nth executable
install(TARGETS nth
        EXPORT NthEngineTargets
        RUNTIME DESTINATION ${NTHENGINE_INSTALL_BINDIR}
        COMPONENT Tools
        BUNDLE DESTINATION ${NTHENGINE_INSTALL_BINDIR}
        COMPONENT Tools
)

# On Windows, also install required DLLs if they exist
if (WIN32)
    # Install NthEngine DLL (it will be in the same bin directory)
    install(FILES $<TARGET_FILE:NthEngine>
            DESTINATION ${NTHENGINE_INSTALL_BINDIR}
            COMPONENT Runtime
    )
endif ()
