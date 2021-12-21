include_directories(
    ${PROJECT_SOURCE_DIR}/core
)

add_executable(vca_command
    src/command.cpp
)

target_link_libraries(vca_command
    restclient-cpp
    served
    vca_core
)
