add_executable(vca_daemon
    src/daemon.cpp
    src/scan.h
    src/scan.cpp
)

include_directories(
    ${PROJECT_SOURCE_DIR}/core
)

target_link_libraries(vca_daemon
    efsw
    gcl
    vca_core
)
