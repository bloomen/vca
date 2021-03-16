add_library(vca_core
    vca/result.h
    vca/result.cpp
    vca/utils.h
)

add_executable(vca_core_test
    test/core_test.cpp
    test/result_test.cpp
)

target_include_directories(vca_core_test
    PUBLIC ${PROJECT_SOURCE_DIR}/core
)

target_link_libraries(vca_core_test
    gtest
    vca_core
)
