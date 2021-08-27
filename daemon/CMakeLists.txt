include_directories(
    ${PROJECT_SOURCE_DIR}/core
)

add_library(vca_daemon_lib
    src/file_processor.h
    src/file_processor.cpp
    src/file_scanner.h
    src/file_scanner.cpp
    src/file_watcher.h
    src/file_watcher.cpp
    src/pdf_tokenizer.h
    src/pdf_tokenizer.cpp
    src/tokenizer.h
    src/txt_tokenizer.h
    src/txt_tokenizer.cpp
    src/xml_tokenizer.h
    src/xml_tokenizer.cpp
    src/zipxml_tokenizer.h
    src/zipxml_tokenizer.cpp
)

target_link_libraries(vca_daemon_lib
    efsw
    gcl
    served
    vca_core
    zip
)

add_executable(vca_daemon
    src/daemon.cpp
)

target_link_libraries(vca_daemon
    vca_daemon_lib
)

add_executable(vca_daemon_test
    test/daemon_test.cpp
)

target_include_directories(vca_daemon_test
    PRIVATE ${PROJECT_SOURCE_DIR}/daemon/src
)

target_link_libraries(vca_daemon_test
    gtest
    vca_daemon_lib
)
