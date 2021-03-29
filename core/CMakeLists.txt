add_library(vca_core
    vca/filesystem.h
    vca/logging.h
    vca/logging.cpp
    vca/sqlite_userdb.h
    vca/sqlite_userdb.cpp
    vca/userdb.h
    vca/utils.h
    vca/utils.cpp
)

target_link_libraries(vca_core PUBLIC
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
