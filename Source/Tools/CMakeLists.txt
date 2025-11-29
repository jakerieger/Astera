project(NthEngine)

option(BUILD_NTHPAK "Build NthPak" ON)

if (BUILD_NTHPAK)
    add_subdirectory(NthPak)
endif ()