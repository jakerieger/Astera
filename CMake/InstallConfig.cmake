# ============================================================================
# Installation Configuration for Astera
# ============================================================================

include(GNUInstallDirs)

# ----------------------------------------------------------------------------
# Install NthCLI Binary
# ----------------------------------------------------------------------------
install(TARGETS astera-cli
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT cli
)

# ----------------------------------------------------------------------------
# Install Engine Libraries
# ----------------------------------------------------------------------------
install(TARGETS Astera_Shared Astera_Static
    EXPORT AsteraTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT libraries
)

# ----------------------------------------------------------------------------
# Install Engine Headers
# ----------------------------------------------------------------------------
install(DIRECTORY ${ENGINE_ROOT}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Astera
    COMPONENT headers
    FILES_MATCHING
    PATTERN "*.hpp"
    PATTERN "*.h"
    PATTERN "*.inl"
)

# Install Common headers (excluding precompiled header)
install(DIRECTORY ${COMMON_ROOT}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Astera/Common
    COMPONENT headers
    FILES_MATCHING
    PATTERN "*.hpp"
    PATTERN "*.h"
    PATTERN "*.inl"
)

# Install necessary vendor headers
install(DIRECTORY ${VENDOR_ROOT}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Astera/Vendor
    COMPONENT headers
    FILES_MATCHING
    PATTERN "*.hpp"
    PATTERN "*.h"
    PATTERN "*.inl"
)

# ----------------------------------------------------------------------------
# Install EngineContent Directory
# ----------------------------------------------------------------------------
install(DIRECTORY ${CMAKE_SOURCE_DIR}/EngineContent/
    DESTINATION ${CMAKE_INSTALL_DATADIR}/Astera/EngineContent
    COMPONENT engine_content
    PATTERN ".git" EXCLUDE
    PATTERN ".DS_Store" EXCLUDE
)

# ----------------------------------------------------------------------------
# CMake Package Configuration
# ----------------------------------------------------------------------------
# Export targets without namespace - we'll add aliases in the config file
install(EXPORT AsteraTargets
    FILE AsteraTargets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Astera
    COMPONENT cmake_config
)

include(CMakePackageConfigHelpers)

# Create package config file
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/CMake/AsteraConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/AsteraConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Astera
    PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR CMAKE_INSTALL_DATADIR
)

# Create version file
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/AsteraConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/AsteraConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/AsteraConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Astera
    COMPONENT cmake_config
)

# ----------------------------------------------------------------------------
# Installation Summary
# ----------------------------------------------------------------------------
message(STATUS "")
message(STATUS "========================================")
message(STATUS "Astera Installation Configuration")
message(STATUS "========================================")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")
message(STATUS "Components:")
message(STATUS "  CLI tool (nth): ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/nth")
message(STATUS "  Libraries: ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
message(STATUS "  Headers: ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}/Astera")
message(STATUS "  Engine content: ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}/Astera/EngineContent")
message(STATUS "  CMake config: ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/cmake/Astera")
message(STATUS "")
message(STATUS "To install, run: cmake --build . --target install")
message(STATUS "========================================")
message(STATUS "")

# ----------------------------------------------------------------------------
# CPack Configuration for Package Generation
# ----------------------------------------------------------------------------
set(CPACK_PACKAGE_NAME "Astera")
set(CPACK_PACKAGE_VENDOR "Jake Rieger")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Astera - Simple 2D game engine")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Jake Rieger <contact.jakerieger@gmail.com>")

if (WIN32)
    set(CPACK_GENERATOR "ZIP;NSIS")
elseif (APPLE)
    set(CPACK_GENERATOR "TGZ;DragNDrop")
else ()
    set(CPACK_GENERATOR "TGZ;DEB;RPM")
endif ()

include(CPack)