project(NthEngine)

file(GLOB ENGINE_SOURCES *.hpp *.cpp)

add_library(NthEngine STATIC
        ../Common/CommonPCH.hpp
        ../Common/CommonPCH.cpp
        ${ENGINE_SOURCES}
)

target_precompile_headers(NthEngine PRIVATE ../Common/CommonPCH.hpp)

target_include_directories(NthEngine PRIVATE
        ../Common
)

target_link_libraries(NthEngine PUBLIC
        glfw
        glm::glm
        spdlog::spdlog
)

if (UNIX AND NOT APPLE)
    target_link_libraries(NthEngine PUBLIC rt)
endif ()
