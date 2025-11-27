project(NthEngine)

# Platform detection and configuration
if (WIN32)
    # Windows platform
    add_compile_definitions(N_ENGINE_PLATFORM_WINDOWS)
    message(STATUS "Platform: Windows")

elseif (APPLE)
    # macOS platform
    add_compile_definitions(N_ENGINE_PLATFORM_MACOS)
    message(STATUS "Platform: macOS")

elseif (UNIX)
    # Linux platform - need to detect X11 vs Wayland

    # Check for Wayland first (as it's becoming more common)
    find_package(PkgConfig)
    if (PkgConfig_FOUND)
        pkg_check_modules(WAYLAND wayland-client)
    endif ()

    # You can also check environment variables
    if (DEFINED ENV{WAYLAND_DISPLAY})
        set(WAYLAND_DETECTED TRUE)
    endif ()

    # Prefer Wayland if available, otherwise default to X11
    # You can add an option to let users choose
    option(N_ENGINE_USE_WAYLAND "Use Wayland instead of X11 on Linux" ${WAYLAND_DETECTED})

    if (N_ENGINE_USE_WAYLAND AND WAYLAND_DETECTED)
        add_compile_definitions(N_ENGINE_PLATFORM_LINUX_WAYLAND)
        message(STATUS "Platform: Linux (Wayland)")
    else ()
        add_compile_definitions(N_ENGINE_PLATFORM_LINUX_X11)
        message(STATUS "Platform: Linux (X11)")
    endif ()

else ()
    message(FATAL_ERROR "Unsupported platform")
endif ()