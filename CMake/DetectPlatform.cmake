if (WIN32)
    # Windows (including Win64)
    add_compile_definitions(ASTERA_PLATFORM_WINDOWS)
    message(STATUS "Detected OS: Windows")
elseif (APPLE)
    # macOS
    add_compile_definitions(ASTERA_PLATFORM_MACOS)
    message(STATUS "Detected OS: macOS")
elseif (UNIX)
    # Linux and other Unix-like systems
    add_compile_definitions(ASTERA_PLATFORM_LINUX)
    message(STATUS "Detected OS: Linux/Unix")
else ()
    # Unknown platform
    message(FATAL_ERROR "Unknown operating system detected")
endif ()