cmake_minimum_required(VERSION 3.14)
project(NthEngine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# This is required on Debian in order to use C++23 features, even after upgrading Clang
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-stdlib=libc++)
    add_link_options(-stdlib=libc++)
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Detect platform and set engine defines (N_ENGINE_XXX)
include(CMake/DetectPlatform.cmake)

# Fetch 3rd party dependencies
include(CMake/FetchDeps.cmake)

add_subdirectory(Source/Engine)
add_subdirectory(Source/Sandbox)
add_subdirectory(Source/Tools)