add_executable(vca_daemon
    src/main.cpp
)

include_directories(
    ${PROJECT_SOURCE_DIR}/core
)

target_link_libraries(vca_daemon
    gcl
    vca_core
)
