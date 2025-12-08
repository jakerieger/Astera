cmake_minimum_required(VERSION 3.14)
project(NthEngine VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Installation directories
include(GNUInstallDirs)
set(NTHENGINE_INSTALL_BINDIR ${CMAKE_INSTALL_BINDIR})
set(NTHENGINE_INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR})
set(NTHENGINE_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR})
set(NTHENGINE_INSTALL_DATADIR ${CMAKE_INSTALL_DATADIR}/NthEngine)
set(NTHENGINE_INSTALL_CMAKEDIR ${CMAKE_INSTALL_LIBDIR}/cmake/NthEngine)

# Detect platform and set engine defines (N_ENGINE_XXX)
include(CMake/DetectPlatform.cmake)

# Fetch 3rd party dependencies
include(CMake/FetchDeps.cmake)
set(NTHENGINE_LIBS
        glfw
        glm::glm
        spdlog::spdlog
        EnTT::EnTT
        fmt::fmt
        pugixml
        luajit
        sol2::sol2
)

add_subdirectory(Source/Engine)
add_subdirectory(Source/Tools)
add_subdirectory(Sandbox/Runtime)

# ============================================================================
# Installation Configuration
# ============================================================================

# Install EngineContent directory
install(DIRECTORY ${CMAKE_SOURCE_DIR}/EngineContent/
        DESTINATION ${NTHENGINE_INSTALL_DATADIR}/EngineContent
        FILES_MATCHING
        PATTERN "*.lua"
        PATTERN "*.frag"
        PATTERN "*.vert"
)

# Install CMake package configuration files
include(CMakePackageConfigHelpers)

# Generate the config file that includes the exports
configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/CMake/NthEngineConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/NthEngineConfig.cmake
        INSTALL_DESTINATION ${NTHENGINE_INSTALL_CMAKEDIR}
        PATH_VARS NTHENGINE_INSTALL_INCLUDEDIR NTHENGINE_INSTALL_LIBDIR NTHENGINE_INSTALL_DATADIR
)

# Generate version file
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/NthEngineConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)

# Install the configuration files
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/NthEngineConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/NthEngineConfigVersion.cmake
        DESTINATION ${NTHENGINE_INSTALL_CMAKEDIR}
)

# Export targets to a script
install(EXPORT NthEngineTargets
        FILE NthEngineTargets.cmake
        NAMESPACE NthEngine::
        DESTINATION ${NTHENGINE_INSTALL_CMAKEDIR}
)

# Generate and install pkg-config file (optional, for Linux)
if (UNIX AND NOT APPLE)
    configure_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/CMake/NthEngine.pc.in
            ${CMAKE_CURRENT_BINARY_DIR}/NthEngine.pc
            @ONLY
    )
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/NthEngine.pc
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
    )
endif ()
