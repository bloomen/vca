add_library(vca_core
    vca/utils.h
    vca/utils.cpp
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
