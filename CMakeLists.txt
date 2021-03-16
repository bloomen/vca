cmake_minimum_required(VERSION 3.0)
project(vca)

add_subdirectory(third_party/gcl)

include_directories(
    third_party/gcl/src
)

if (MSVC)
    add_compile_options(/std:c++17 /W4 /bigobj /EHsc /wd4503 /wd4996 /wd4702 /wd4100 /wd4706)
else()
    add_compile_options(-std=c++17 -pedantic -Wall -Wextra -Wconversion)
endif()

add_subdirectory(core)
add_subdirectory(daemon)
add_subdirectory(desktop)
