cmake_minimum_required(VERSION 3.13)
project(vca)

option(vca_enable_asan "Build vca with address sanitizer." OFF)
option(vca_enable_tsan "Build vca with thread sanitizer." OFF)

if(vca_enable_asan AND vca_enable_tsan)
    message(FATAL_ERROR "vca_enable_asan and vca_enable_tsan cannot both be ON")
endif()

if(MSVC)
list(APPEND CMAKE_PREFIX_PATH "C:\\local\\curl_x64-windows")
list(APPEND CMAKE_PREFIX_PATH "C:\\local\\zlib_x64-windows")
set(BOOST_ROOT "C:\\local\\boost_1_71_0")
set(BOOST_LIBRARYDIR "C:\\local\\boost_1_71_0\\lib64-msvc-14.1")
add_definitions(-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BUILD_SHARED_LIBS OFF)
set(SERVED_BUILD_SHARED OFF)
set(SERVED_BUILD_STATIC ON)
set(SERVED_BUILD_TESTS OFF)
set(SERVED_BUILD_EXAMPLES OFF)
#set(SOCKPP_BUILD_SHARED OFF)
#set(SOCKPP_BUILD_STATIC ON)
set(SQLITECPP_RUN_CPPLINT OFF)
set(SQLITE_ENABLE_COLUMN_METADATA)
set(BUILD_TESTING OFF)
set(CMAKE_DISABLE_TESTING ON)

if(MSVC)
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
add_subdirectory(third_party/restclient-cpp)
add_subdirectory(third_party/served)
#add_subdirectory(third_party/sockpp)
add_subdirectory(third_party/spdlog)
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
    third_party/restclient-cpp/include
    third_party/served/src
#    third_party/sockpp/include
    third_party/spdlog/include
    third_party/SQLiteCpp/include
    third_party/zip/src
)

if(MSVC)
    add_compile_options(/W4 /bigobj /EHsc /wd4503 /wd4996 /wd4702 /wd4100 /wd4706 /wd26812 /wd4373)
    if(${MSVC_VERSION} GREATER_EQUAL 1929)
        if(vca_enable_asan)
            add_compile_options(/fsanitize=address)
        endif()
    endif()
else()
    add_compile_options(-Wall -Wconversion -Wextra -Wpedantic)
    if(vca_enable_asan)
        add_compile_options(-fsanitize=address,leak,undefined)
        add_link_options(-fsanitize=address,leak,undefined)
    endif()
    if(vca_enable_tsan)
        add_compile_options(-fsanitize=thread)
        add_link_options(-fsanitize=thread)
    endif()
endif()

add_subdirectory(core)
add_subdirectory(command)
add_subdirectory(daemon)
add_subdirectory(desktop)
