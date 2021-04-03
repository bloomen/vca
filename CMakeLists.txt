cmake_minimum_required(VERSION 3.0)
project(vca)

set(CMAKE_CXX_STANDARD 17)

set(BUILD_SHARED_LIBS OFF)
set(SQLITECPP_RUN_CPPLINT OFF)
set(SQLITE_ENABLE_COLUMN_METADATA)

if (MSVC)
    set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
     )
     foreach(CompilerFlag ${CompilerFlags})
        string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
     endforeach()
endif()

add_subdirectory(third_party/concurrentqueue)
add_subdirectory(third_party/efsw)
add_subdirectory(third_party/gcl)
add_subdirectory(third_party/googletest)
add_subdirectory(third_party/json)
add_subdirectory(third_party/readerwriterqueue)
add_subdirectory(third_party/spdlog)
add_subdirectory(third_party/SQLiteCpp)

find_package(Boost 1.71.0 COMPONENTS date_time)
add_definitions(-DBOOST_ALL_NO_LIB)
add_definitions(-DVCA_SOURCE_DIR="${PROJECT_SOURCE_DIR}")

include_directories(
    ${Boost_INCLUDE_DIRS}
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
    add_compile_options(/W4 /bigobj /EHsc /wd4503 /wd4996 /wd4702 /wd4100 /wd4706 /wd26812 /wd4373)
else()
    add_compile_options(-Wpedantic -Wall -Wextra -Wconversion)
    if (CMAKE_BUILD_TYPE MATCHES DEBUG)
        add_compile_options(-fsanitize=address)
        add_link_options(-fsanitize=address)
    endif()
endif()

add_subdirectory(core)
add_subdirectory(daemon)
add_subdirectory(desktop)
