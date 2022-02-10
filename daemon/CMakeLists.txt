add_library(vca_daemon_lib
    src/file_processor.h
    src/file_processor.cpp
    src/file_scanner.h
    src/file_scanner.cpp
    src/file_watcher.h
    src/file_watcher.cpp
    src/http_server.h
    src/http_server.cpp
    src/pdf_tokenizer.h
    src/pdf_tokenizer.cpp
    src/tokenizer.h
    src/tex_tokenizer.h
    src/tex_tokenizer.cpp
    src/txt_tokenizer.h
    src/txt_tokenizer.cpp
    src/xml_tokenizer.h
    src/xml_tokenizer.cpp
    src/zipxml_tokenizer.h
    src/zipxml_tokenizer.cpp
)

target_include_directories(vca_daemon_lib
    PUBLIC ${PROJECT_SOURCE_DIR}/core
    PUBLIC ${PROJECT_SOURCE_DIR}/third_party/served/src
)

target_link_libraries(vca_daemon_lib
    opencv::opencv
    podofo::podofo
    served
    vca_core
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
    GTest::GTest
    vca_daemon_lib
)

if(MSVC)

add_custom_command(TARGET vca_daemon POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CONAN_PODOFO_ROOT}/bin"
    $<TARGET_FILE_DIR:vca_daemon>)

add_custom_command(TARGET vca_daemon_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CONAN_PODOFO_ROOT}/bin"
    $<TARGET_FILE_DIR:vca_daemon_test>)

endif()
