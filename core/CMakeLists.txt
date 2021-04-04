add_library(vca_core
    vca/config.h
    vca/config.cpp
    vca/file_lock.h
    vca/file_lock.cpp
    vca/filesystem.h
    vca/filesystem.cpp
    vca/logging.h
    vca/logging.cpp
    vca/platform.h
    vca/sqlite_userdb.h
    vca/sqlite_userdb.cpp
    vca/time.h
    vca/userdb.h
    vca/utils.h
    vca/utils.cpp
)

target_link_libraries(vca_core PUBLIC
    ${Boost_LIBRARIES}
    nlohmann_json
    platform_folders
    spdlog
    sqlite3
    SQLiteCpp
)

add_executable(vca_core_test
    test/core_test.cpp
    test/utils_test.cpp
)

target_include_directories(vca_core_test
    PRIVATE ${PROJECT_SOURCE_DIR}/core
)

target_link_libraries(vca_core_test
    gtest
    vca_core
)
