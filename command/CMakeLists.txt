include_directories(
    ${PROJECT_SOURCE_DIR}/core
)

add_executable(vca_command
    src/command.cpp
)

target_include_directories(vca_command
    PRIVATE ${PROJECT_SOURCE_DIR}/third_party/restclient-cpp/include
    PRIVATE ${PROJECT_SOURCE_DIR}/third_party/served/src
)

target_link_libraries(vca_command
    restclient-cpp
    served
    vca_core
)
