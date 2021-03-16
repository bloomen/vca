cmake_minimum_required(VERSION 3.0)
project(vca)

add_subdirectory(third_party/gcl EXCLUDE_FROM_ALL)
add_subdirectory(third_party/googletest EXCLUDE_FROM_ALL)

include_directories(
    third_party/gcl/src
    third_party/googletest/googletest/include
)

if (MSVC)
    add_compile_options(/std:c++17 /W4 /bigobj /EHsc /wd4503 /wd4996 /wd4702 /wd4100 /wd4706)
else()
    add_compile_options(-std=c++17 -pedantic -Wall -Wextra -Wconversion)
endif()

add_subdirectory(core)
add_subdirectory(daemon)
add_subdirectory(desktop)
