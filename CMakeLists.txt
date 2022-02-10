cmake_minimum_required(VERSION 3.13)
project(vca)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

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
set(BUILD_TESTING OFF)
set(CMAKE_DISABLE_TESTING ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(MSVC)
    add_definitions(-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
endif()

find_package(Threads REQUIRED)

include(third_party/conan.cmake/conan.cmake)

conan_cmake_autodetect(settings BUILD_TYPE ${CMAKE_BUILD_TYPE})
conan_cmake_install(PATH_OR_REFERENCE ${PROJECT_SOURCE_DIR}
                    REMOTE conancenter
                    SETTINGS ${settings})

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

find_package(Boost 1.78 COMPONENTS filesystem REQUIRED system REQUIRED)
find_package(concurrentqueue REQUIRED)
find_package(cryptopp REQUIRED)
find_package(GTest REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(OpenCV REQUIRED)
find_package(podofo REQUIRED)
find_package(Qt6 COMPONENTS Qml REQUIRED Quick REQUIRED)
find_package(spdlog REQUIRED)
find_package(SQLiteCpp REQUIRED)
find_package(zip REQUIRED)

add_subdirectory(third_party/efsw)
add_subdirectory(third_party/Format.cmake)
add_subdirectory(third_party/PlatformFolders)
add_subdirectory(third_party/restclient-cpp)
add_subdirectory(third_party/served)

add_definitions(-DVCA_SOURCE_DIR="${PROJECT_SOURCE_DIR}")

if(MSVC)
    add_compile_options(/utf-8 /WX /W4 /bigobj /EHsc /wd4503 /wd4996 /wd4702 /wd4100 /wd4706 /wd26812 /wd4373)
    if(${MSVC_VERSION} GREATER_EQUAL 1929)
        if(vca_enable_asan)
            add_compile_options(/fsanitize=address)
        endif()
    endif()
    add_definitions(/FI"${PROJECT_SOURCE_DIR}/core/vca/platform.h")
    add_link_options(/ignore:4099)
else()
    add_compile_options(-Werror -Wall -Wconversion -Wextra -Wpedantic)
    if(vca_enable_asan)
        add_compile_options(-fsanitize=address,leak,undefined)
        add_link_options(-fsanitize=address,leak,undefined)
    endif()
    if(vca_enable_tsan)
        add_compile_options(-fsanitize=thread)
        add_link_options(-fsanitize=thread)
    endif()
    add_definitions(-include ${PROJECT_SOURCE_DIR}/core/vca/platform.h)
endif()

add_subdirectory(core)
add_subdirectory(command)
add_subdirectory(daemon)
add_subdirectory(desktop)

set_target_properties(
BeginMessage
check-clang-format
check-cmake-format
check-format
clang-format
cmake-format
fix-clang-format
fix-cmake-format
fix-format
format
PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
