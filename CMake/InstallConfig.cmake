# ============================================================================
# Installation Configuration for NthEngine
# ============================================================================

include(GNUInstallDirs)

# ----------------------------------------------------------------------------
# Install NthCLI Binary
# ----------------------------------------------------------------------------
install(TARGETS nth
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT cli
)

# ----------------------------------------------------------------------------
# Install Engine Libraries
# ----------------------------------------------------------------------------
install(TARGETS NthEngine_Shared NthEngine_Static
    EXPORT NthEngineTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT libraries
)

# ----------------------------------------------------------------------------
# Install Engine Headers
# ----------------------------------------------------------------------------
install(DIRECTORY ${ENGINE_ROOT}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/NthEngine
    COMPONENT headers
    FILES_MATCHING
    PATTERN "*.hpp"
    PATTERN "*.h"
)

# Install Common headers (excluding precompiled header)
install(DIRECTORY ${COMMON_ROOT}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/NthEngine/Common
    COMPONENT headers
    FILES_MATCHING
    PATTERN "*.hpp"
    PATTERN "*.h"
    PATTERN "CommonPCH.hpp" EXCLUDE
    PATTERN "CommonPCH.cpp" EXCLUDE
)

# Install necessary vendor headers
install(DIRECTORY ${VENDOR_ROOT}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/NthEngine/Vendor
    COMPONENT headers
    FILES_MATCHING
    PATTERN "*.hpp"
    PATTERN "*.h"
)

# ----------------------------------------------------------------------------
# Install EngineContent Directory
# ----------------------------------------------------------------------------
install(DIRECTORY ${CMAKE_SOURCE_DIR}/EngineContent/
    DESTINATION ${CMAKE_INSTALL_DATADIR}/NthEngine/EngineContent
    COMPONENT engine_content
    PATTERN ".git" EXCLUDE
    PATTERN ".DS_Store" EXCLUDE
)

# ----------------------------------------------------------------------------
# CMake Package Configuration
# ----------------------------------------------------------------------------
install(EXPORT NthEngineTargets
    FILE NthEngineTargets.cmake
    NAMESPACE NthEngine::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/NthEngine
    COMPONENT cmake_config
)

include(CMakePackageConfigHelpers)

# Create package config file
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/CMake/NthEngineConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/NthEngineConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/NthEngine
    PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR CMAKE_INSTALL_DATADIR
)

# Create version file
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/NthEngineConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/NthEngineConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/NthEngineConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/NthEngine
    COMPONENT cmake_config
)

# ----------------------------------------------------------------------------
# Installation Summary
# ----------------------------------------------------------------------------
message(STATUS "")
message(STATUS "========================================")
message(STATUS "NthEngine Installation Configuration")
message(STATUS "========================================")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")
message(STATUS "Components:")
message(STATUS "  CLI tool (nth): ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/nth")
message(STATUS "  Libraries: ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
message(STATUS "  Headers: ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}/NthEngine")
message(STATUS "  Engine content: ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}/NthEngine/EngineContent")
message(STATUS "  CMake config: ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/cmake/NthEngine")
message(STATUS "")
message(STATUS "To install, run: cmake --build . --target install")
message(STATUS "========================================")
message(STATUS "")

# ----------------------------------------------------------------------------
# Optional: CPack Configuration for Package Generation
# ----------------------------------------------------------------------------
set(CPACK_PACKAGE_NAME "NthEngine")
set(CPACK_PACKAGE_VENDOR "Jake Rieger")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "NthEngine - Simple 2D game engine")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

# Configure package generators based on platform
if (WIN32)
    set(CPACK_GENERATOR "ZIP;NSIS")
elseif (APPLE)
    set(CPACK_GENERATOR "TGZ;DragNDrop")
else ()
    set(CPACK_GENERATOR "TGZ;DEB;RPM")
endif ()

include(CPack)