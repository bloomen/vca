cmake_minimum_required(VERSION 3.0)
project(vca)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BUILD_SHARED_LIBS OFF)
set(SQLITECPP_RUN_CPPLINT OFF)
set(BUILD_TESTING OFF)
set(CMAKE_DISABLE_TESTING OFF)
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
add_subdirectory(third_party/nlohmann_json)
add_subdirectory(third_party/PlatformFolders)
add_subdirectory(third_party/pugixml)
add_subdirectory(third_party/spdlog)
add_subdirectory(third_party/tiny-utf8)
add_subdirectory(third_party/SQLiteCpp)
add_subdirectory(third_party/zip)

include_directories(
    third_party/zlib
)
add_subdirectory(third_party/zlib)

find_package(Boost 1.71.0 COMPONENTS date_time)
add_definitions(-DBOOST_ALL_NO_LIB)
add_definitions(-DVCA_SOURCE_DIR="${PROJECT_SOURCE_DIR}")

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR}/third_party/zlib
    third_party/concurrentqueue
    third_party/efsw/include
    third_party/gcl/include
    third_party/googletest/googletest/include
    third_party/nlohmann_json/single_include
    third_party/PlatformFolders
    third_party/pugixml/src
    third_party/spdlog/include
    third_party/tiny-utf8/include
    third_party/SQLiteCpp/include
    third_party/zip/src
)

if (MSVC)
    add_compile_options(/W4 /bigobj /EHsc /wd4503 /wd4996 /wd4702 /wd4100 /wd4706 /wd26812 /wd4373)
else()
    add_compile_options(-Wpedantic -Wall -Wextra -Wconversion)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-fsanitize=address,leak,undefined)
        add_link_options(-fsanitize=address,leak,undefined)
    endif()
endif()

add_subdirectory(core)
add_subdirectory(daemon)
add_subdirectory(command)
