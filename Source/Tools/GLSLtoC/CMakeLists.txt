set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Tools/GLSLtoC)

add_executable(glsl2c main.cpp)

target_include_directories(glsl2c PRIVATE
    ${VENDOR_ROOT}
    ${CMAKE_CURRENT_SOURCE_DIR}
)