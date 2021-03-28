add_executable(vca_daemon
    src/daemon.cpp
)

include_directories(
    ${PROJECT_SOURCE_DIR}/core
)

target_link_libraries(vca_daemon
    efsw
    gcl
    SQLiteCpp
    sqlite3
    vca_core
)
