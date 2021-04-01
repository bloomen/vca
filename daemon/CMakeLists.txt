add_executable(vca_daemon
    src/daemon.cpp
    src/default_tokenizer.h
    src/file_processor.h
    src/file_processor.cpp
    src/file_scanner.h
    src/file_scanner.cpp
    src/file_watcher.h
    src/file_watcher.cpp
    src/tokenizer.h
)

include_directories(
    ${PROJECT_SOURCE_DIR}/core
)

target_link_libraries(vca_daemon
    efsw
    gcl
    vca_core
)
