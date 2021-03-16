cmake_minimum_required(VERSION 3.0)
project(vca)

add_subdirectory(third_party/gcl)

include_directories(
    third_party/gcl/src
)

find_package(Threads)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /W4 /bigobj /EHsc /wd4503 /wd4996 /wd4702 /wd4100 /wd4706")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pedantic -Wall -Wextra -Wconversion")
    if (CMAKE_COMPILER_IS_GNUCC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    endif()
    set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} ${CMAKE_THREAD_LIBS_INIT}")
endif()

add_subdirectory(core)
add_subdirectory(daemon)
add_subdirectory(desktop)
