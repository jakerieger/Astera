file(GLOB ENGINE_SOURCES
    *.cpp
    **/*.cpp
)

file(GLOB ENGINE_INCLUDES
    *.hpp
    **/*.hpp
)

add_library(NthEngine_Objects OBJECT
    ${IMGUI_SOURCES}
    ${MINIAUDIO_SOURCES}
    ${GLAD_SOURCES}
    ${STB_SOURCES}
    ${ENGINE_INCLUDES}
    ${ENGINE_SOURCES}
)

target_include_directories(NthEngine_Objects PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${COMMON_ROOT}>
    $<BUILD_INTERFACE:${SOURCE_ROOT}>
    $<BUILD_INTERFACE:${VENDOR_ROOT}>
    $<INSTALL_INTERFACE:include/NthEngine>
)

# CRITICAL: Enable PIC for the object library
set_target_properties(NthEngine_Objects PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

# Link dependencies to the object library
target_link_libraries(NthEngine_Objects PUBLIC ${NTHENGINE_LIBS})

# Create shared library from object files
add_library(NthEngine_Shared SHARED $<TARGET_OBJECTS:NthEngine_Objects>)
set_target_properties(NthEngine_Shared PROPERTIES OUTPUT_NAME NthEngine)

# Create static library from object files
add_library(NthEngine_Static STATIC $<TARGET_OBJECTS:NthEngine_Objects>)
set_target_properties(NthEngine_Static PROPERTIES OUTPUT_NAME NthEngine)

# Inherit include directories and compile definitions, but use INTERFACE linking
# This breaks the direct dependency chain for exports
target_include_directories(NthEngine_Shared PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${COMMON_ROOT}>
    $<BUILD_INTERFACE:${SOURCE_ROOT}>
    $<BUILD_INTERFACE:${VENDOR_ROOT}>
    $<INSTALL_INTERFACE:include/NthEngine>
)

target_include_directories(NthEngine_Static PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${COMMON_ROOT}>
    $<BUILD_INTERFACE:${SOURCE_ROOT}>
    $<BUILD_INTERFACE:${VENDOR_ROOT}>
    $<INSTALL_INTERFACE:include/NthEngine>
)

# Link the actual libraries (not the object library) to dependencies
target_link_libraries(NthEngine_Shared PUBLIC ${NTHENGINE_LIBS})
target_link_libraries(NthEngine_Static PUBLIC ${NTHENGINE_LIBS})

include(GenerateExportHeader)
generate_export_header(NthEngine_Shared)

# Aliases
add_library(NthEngine::Shared ALIAS NthEngine_Shared)
add_library(NthEngine::Static ALIAS NthEngine_Static)
add_library(NthEngine::NthEngine ALIAS NthEngine_Shared)