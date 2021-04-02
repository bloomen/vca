cmake_minimum_required(VERSION 3.0)
project(vca)

set(BUILD_SHARED_LIBS "FALSE")
set(SQLITECPP_RUN_CPPLINT "OFF")
set(SQLITE_ENABLE_COLUMN_METADATA)

add_subdirectory(third_party/concurrentqueue)
add_subdirectory(third_party/efsw)
add_subdirectory(third_party/gcl)
add_subdirectory(third_party/googletest)
add_subdirectory(third_party/json)
add_subdirectory(third_party/readerwriterqueue)
add_subdirectory(third_party/spdlog)
add_subdirectory(third_party/SQLiteCpp)

include_directories(
    third_party/boost_header_only
    third_party/concurrentqueue
    third_party/efsw/include
    third_party/gcl/include
    third_party/googletest/googletest/include
    third_party/json/single_include
    third_party/readerwriterqueue
    third_party/spdlog/include
    third_party/SQLiteCpp/include
)

if (MSVC)
    add_compile_options(/std:c++17 /W4 /bigobj /EHsc /wd4503 /wd4996 /wd4702 /wd4100 /wd4706)
else()
    add_compile_options(-std=c++17 -pedantic -Wall -Wextra -Wconversion)
    if (CMAKE_BUILD_TYPE MATCHES DEBUG)
        add_compile_options(-fsanitize=address)
        add_link_options(-fsanitize=address)
    endif()
endif()

add_subdirectory(core)
add_subdirectory(daemon)
add_subdirectory(desktop)
