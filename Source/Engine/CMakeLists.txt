project(NthEngine)

file(GLOB ENGINE_SOURCES
        *.hpp
        *.cpp
        **/*.hpp
        **/*.cpp
)

add_library(NthEngine SHARED
        ../Common/CommonPCH.hpp
        ../Common/CommonPCH.cpp
        ../Vendor/glad.c
        ../Vendor/stb_image.cpp
        ${ENGINE_SOURCES}
)
include(GenerateExportHeader)
generate_export_header(NthEngine)

target_precompile_headers(NthEngine PRIVATE ../Common/CommonPCH.hpp)

# Remove glad from PCH
set_source_files_properties(../Vendor/glad.c PROPERTIES
        SKIP_PRECOMPILE_HEADERS ON
        LANGUAGE C
)

target_include_directories(NthEngine PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Source>
        $<INSTALL_INTERFACE:include/NthEngine>
)

target_link_libraries(NthEngine PRIVATE
        ${NTHENGINE_LIBS}
)

if (UNIX AND NOT APPLE)
    target_link_libraries(NthEngine PUBLIC rt dl m)
endif ()

if (WIN32)
    add_compile_definitions(
            NOMINMAX
            WIN32_LEAN_AND_MEAN
            VC_EXTRALEAN
    )
endif ()

# ============================================================================
# Installation Configuration
# ============================================================================

# Install the library
install(TARGETS NthEngine
        EXPORT NthEngineTargets
        RUNTIME DESTINATION ${NTHENGINE_INSTALL_BINDIR}
        COMPONENT Runtime
        LIBRARY DESTINATION ${NTHENGINE_INSTALL_LIBDIR}
        COMPONENT Runtime
        ARCHIVE DESTINATION ${NTHENGINE_INSTALL_LIBDIR}
        COMPONENT Development
        INCLUDES DESTINATION ${NTHENGINE_INSTALL_INCLUDEDIR}
)

# Install public headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
        DESTINATION ${NTHENGINE_INSTALL_INCLUDEDIR}/NthEngine
        COMPONENT Development
        FILES_MATCHING
        PATTERN "*.hpp"
        PATTERN "*.h"
)

# Install Common headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/Source/Common/
        DESTINATION ${NTHENGINE_INSTALL_INCLUDEDIR}/NthEngine/Common
        COMPONENT Development
        FILES_MATCHING
        PATTERN "*.hpp"
        PATTERN "*.h"
        PATTERN "*.cpp" EXCLUDE
)

# Install Vendor headers
install(FILES
        ${CMAKE_SOURCE_DIR}/Source/Vendor/glad.h
        ${CMAKE_SOURCE_DIR}/Source/Vendor/khrplatform.h
        ${CMAKE_SOURCE_DIR}/Source/Vendor/stb_image.h
        DESTINATION ${NTHENGINE_INSTALL_INCLUDEDIR}/NthEngine/Vendor
        COMPONENT Development
)

# Install generated export header
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/nthengine_export.h
        DESTINATION ${NTHENGINE_INSTALL_INCLUDEDIR}/NthEngine
        COMPONENT Development
)
