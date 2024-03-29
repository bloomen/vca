add_library(vca_core
    vca/async.h
    vca/async.cpp
    vca/case_mappings.h
    vca/case_mappings.cpp
    vca/config.h
    vca/config.cpp
    vca/file_lock.h
    vca/file_lock.cpp
    vca/filesystem.h
    vca/filesystem.cpp
    vca/logging.h
    vca/logging.cpp
    vca/platform.h
    vca/command_queue.h
    vca/command_queue.cpp
    vca/sqlite_userdb.h
    vca/sqlite_userdb.cpp
    vca/string.h
    vca/string.cpp
    vca/time.h
    vca/userdb.h
    vca/userdb.cpp
    vca/utils.h
    vca/utils.cpp
    vca/zip_inflater.h
    vca/zip_inflater.cpp
)

target_include_directories(vca_core
    PRIVATE ${PROJECT_SOURCE_DIR}/third_party/efsw/include
    PRIVATE ${PROJECT_SOURCE_DIR}/third_party/PlatformFolders
)

target_link_libraries(vca_core
    Boost::filesystem
    Boost::system
    concurrentqueue::concurrentqueue
    cryptopp::cryptopp
    efsw
    nlohmann_json::nlohmann_json
    platform_folders
    spdlog::spdlog
    SQLiteCpp::SQLiteCpp
    zip::zip
)

add_executable(vca_core_test
    test/core_test.cpp
    test/string_test.cpp
    test/utils_test.cpp
)

target_include_directories(vca_core_test
    PRIVATE ${PROJECT_SOURCE_DIR}/core
)

target_link_libraries(vca_core_test
    GTest::GTest
    vca_core
)
