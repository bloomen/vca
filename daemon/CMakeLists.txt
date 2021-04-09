add_executable(vca_daemon
    src/daemon.cpp
    src/file_processor.h
    src/file_processor.cpp
    src/file_scanner.h
    src/file_scanner.cpp
    src/filename_tokenizer.h
    src/filename_tokenizer.cpp
    src/file_watcher.h
    src/file_watcher.cpp
    src/tokenizer.h
    src/txt_tokenizer.h
    src/txt_tokenizer.cpp
)

include_directories(
    ${PROJECT_SOURCE_DIR}/core
)

target_link_libraries(vca_daemon
    efsw
    gcl
    vca_core
)
