# CPack Configuration for NthEngine Distribution Packages
# Add this to the end of your root CMakeLists.txt

# ============================================================================
# CPack Configuration for Creating Distribution Packages
# ============================================================================

set(CPACK_PACKAGE_NAME "NthEngine")
set(CPACK_PACKAGE_VENDOR "YourCompany")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "NthEngine - A modern game engine")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "NthEngine")

# Contact and licensing
set(CPACK_PACKAGE_CONTACT "your.email@example.com")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")

# Component configuration
set(CPACK_COMPONENTS_ALL Runtime Development Tools)
set(CPACK_COMPONENT_RUNTIME_DISPLAY_NAME "Runtime Libraries")
set(CPACK_COMPONENT_DEVELOPMENT_DISPLAY_NAME "Development Headers")
set(CPACK_COMPONENT_TOOLS_DISPLAY_NAME "Command-line Tools")

set(CPACK_COMPONENT_RUNTIME_DESCRIPTION
        "Essential runtime libraries and engine content")
set(CPACK_COMPONENT_DEVELOPMENT_DESCRIPTION
        "Header files for developing with NthEngine")
set(CPACK_COMPONENT_TOOLS_DESCRIPTION
        "Command-line utilities (nth, nthpak)")

# Dependencies between components
set(CPACK_COMPONENT_DEVELOPMENT_DEPENDS Runtime)
set(CPACK_COMPONENT_TOOLS_DEPENDS Runtime)

# ============================================================================
# Platform-specific CPack Configuration
# ============================================================================

if (WIN32)
    # Windows NSIS Installer
    set(CPACK_GENERATOR "NSIS;ZIP")
    set(CPACK_NSIS_DISPLAY_NAME "NthEngine")
    set(CPACK_NSIS_PACKAGE_NAME "NthEngine")
    set(CPACK_NSIS_HELP_LINK "https://github.com/yourusername/NthEngine")
    set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/yourusername/NthEngine")
    set(CPACK_NSIS_CONTACT "your.email@example.com")
    set(CPACK_NSIS_MODIFY_PATH ON)
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)

    # Add Start Menu shortcuts
    set(CPACK_PACKAGE_EXECUTABLES "nth" "NthEngine CLI")

    # Icon for installer
    # set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/Sandbox/Runtime/Res/icon.ico")
    # set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/Sandbox/Runtime/Res/icon.ico")

elseif (APPLE)
    # macOS Bundle / DMG
    set(CPACK_GENERATOR "DragNDrop;TGZ")
    set(CPACK_DMG_VOLUME_NAME "NthEngine")
    set(CPACK_DMG_FORMAT "UDBZ")

    # Bundle configuration
    set(CPACK_BUNDLE_NAME "NthEngine")
    # set(CPACK_BUNDLE_ICON "${CMAKE_SOURCE_DIR}/Sandbox/Runtime/Res/icon.png")
    set(CPACK_BUNDLE_PLIST "${CMAKE_SOURCE_DIR}/Info.plist")

else ()
    # Linux packages
    set(CPACK_GENERATOR "DEB;RPM;TGZ")

    # Debian/Ubuntu
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_CONTACT}")
    set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
    set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "")
    set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
    set(CPACK_DEBIAN_PACKAGE_CONTROL_STRICT_PERMISSION TRUE)

    # Component-specific Debian packages
    set(CPACK_DEB_COMPONENT_INSTALL ON)
    set(CPACK_DEBIAN_RUNTIME_PACKAGE_NAME "libnthengine")
    set(CPACK_DEBIAN_DEVELOPMENT_PACKAGE_NAME "libnthengine-dev")
    set(CPACK_DEBIAN_TOOLS_PACKAGE_NAME "nthengine-tools")

    # RPM (RedHat/Fedora)
    set(CPACK_RPM_PACKAGE_LICENSE "MIT")
    set(CPACK_RPM_PACKAGE_GROUP "Development/Libraries")
    set(CPACK_RPM_FILE_NAME RPM-DEFAULT)

    # Component-specific RPM packages
    set(CPACK_RPM_COMPONENT_INSTALL ON)
    set(CPACK_RPM_RUNTIME_PACKAGE_NAME "libnthengine")
    set(CPACK_RPM_DEVELOPMENT_PACKAGE_NAME "libnthengine-devel")
    set(CPACK_RPM_TOOLS_PACKAGE_NAME "nthengine-tools")

    # Post-install scripts for ldconfig
    set(CPACK_RPM_RUNTIME_POST_INSTALL_SCRIPT_FILE
            "${CMAKE_SOURCE_DIR}/CMake/rpm_post_install.sh")
    set(CPACK_DEBIAN_RUNTIME_PACKAGE_CONTROL_EXTRA
            "${CMAKE_SOURCE_DIR}/CMake/deb_postinst")
endif ()

# Archive configuration (for source distribution)
set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
set(CPACK_SOURCE_IGNORE_FILES
        "/\\\\.git/"
        "/\\\\.gitignore"
        "/\\\\.gitmodules"
        "/build"
        "/\\\\.vscode/"
        "/\\\\.idea/"
        "\\\\.DS_Store"
        "/CMakeFiles/"
        "/CMakeCache\\\\.txt"
        ".*~$"
)

include(CPack)
