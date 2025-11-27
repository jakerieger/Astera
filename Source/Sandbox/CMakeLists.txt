project(NthEngine)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Sandbox)

add_executable(Sandbox
        main.cpp
)

target_link_libraries(Sandbox PRIVATE NthEngine)
target_include_directories(Sandbox PRIVATE
        ../Engine
        ../Common
)

