cmake_minimum_required(VERSION 3.0)
project(vca)
enable_testing()

include_directories(third-party)

find_package(Threads)

add_executable(vca_daemon 
    src/daemon/main.cpp
)
target_link_libraries(vca_daemon 
    gcl
)

add_executable(vca_desktop 
    src/desktop/main.cpp
)
target_link_libraries(vca_desktop 
    gcl
)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /W4 /bigobj /EHsc /wd4503 /wd4996 /wd4702 /wd4100 /wd4706")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pedantic -Wall -Wextra -Wconversion")
    if (CMAKE_COMPILER_IS_GNUCC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    endif()
    target_link_libraries(vca_daemon ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(vca_desktop ${CMAKE_THREAD_LIBS_INIT})
endif()
