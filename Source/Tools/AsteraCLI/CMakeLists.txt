add_executable(astera-cli
    main.cpp
)

target_link_libraries(astera-cli PRIVATE
    CLI11::CLI11
    Astera::Static
)