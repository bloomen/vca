cmake_minimum_required(VERSION 3.0)
project(vca)

add_subdirectory(third_party/concurrentqueue)
add_subdirectory(third_party/efsw)
add_subdirectory(third_party/gcl)
add_subdirectory(third_party/googletest)
add_subdirectory(third_party/json)
add_subdirectory(third_party/readerwriterqueue)

include_directories(
    third_party/concurrentqueue
    third_party/efsw/include
    third_party/gcl/src
    third_party/googletest/googletest/include
    third_party/json/single_include
    third_party/readerwriterqueue
)

if (MSVC)
    add_compile_options(/std:c++17 /W4 /bigobj /EHsc /wd4503 /wd4996 /wd4702 /wd4100 /wd4706)
else()
    add_compile_options(-std=c++17 -pedantic -Wall -Wextra -Wconversion)
endif()

add_subdirectory(core)
add_subdirectory(daemon)
add_subdirectory(desktop)
