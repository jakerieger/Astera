add_executable(nth
    ProjectDescriptor.hpp
    ProjectRunner.cpp
    ProjectRunner.hpp
    main.cpp
)

target_link_libraries(nth PRIVATE
    CLI11::CLI11
    NthEngine::Static
)

target_include_directories(nth PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SOURCE_ROOT}
    ${VENDOR_ROOT}
    ${ENGINE_ROOT}
    ${COMMON_ROOT}
)