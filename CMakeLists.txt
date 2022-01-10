cmake_minimum_required(VERSION 3.13)
project(vca)

option(vca_enable_asan "Build vca with address sanitizer." OFF)
option(vca_enable_tsan "Build vca with thread sanitizer." OFF)

if(vca_enable_asan AND vca_enable_tsan)
    message(FATAL_ERROR "vca_enable_asan and vca_enable_tsan cannot both be ON")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BUILD_SHARED_LIBS OFF)
set(SERVED_BUILD_SHARED OFF)
set(SERVED_BUILD_STATIC ON)
set(SERVED_BUILD_TESTS OFF)
set(SERVED_BUILD_EXAMPLES OFF)
set(SQLITECPP_RUN_CPPLINT OFF)
set(SQLITE_ENABLE_COLUMN_METADATA)
set(BUILD_TESTING OFF)
set(CMAKE_DISABLE_TESTING ON)

find_package(Threads REQUIRED)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

if(MSVC)
    add_definitions(-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
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

add_subdirectory(third_party/efsw)
add_subdirectory(third_party/PlatformFolders)
add_subdirectory(third_party/restclient-cpp)
add_subdirectory(third_party/served)

include_directories(
    ${CONAN_INCLUDE_DIRS_BOOST}
    ${CONAN_INCLUDE_DIRS_CONCURRENTQUEUE}
    ${CONAN_INCLUDE_DIRS_KUBA_ZIP}
    ${CONAN_INCLUDE_DIRS_LIBCURL}
    ${CONAN_INCLUDE_DIRS_NLOHMANN_JSON}
    ${CONAN_INCLUDE_DIRS_SPDLOG}
    ${CONAN_INCLUDE_DIRS_SQLITECPP}
    ${CONAN_INCLUDE_DIRS_ZLIB}
    ${CONAN_QT_ROOT}/include
    third_party/efsw/include
    third_party/PlatformFolders
    third_party/restclient-cpp/include
    third_party/served/src
)

add_definitions(-DVCA_SOURCE_DIR="${PROJECT_SOURCE_DIR}")

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
