cmake_minimum_required(VERSION 3.16)
project(oop_lab2_string LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_ASAN "Build with AddressSanitizer" OFF)

if(ENABLE_ASAN)
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=address)
endif()

add_library(string_lib STATIC string.cpp)
target_include_directories(string_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(string_lib PRIVATE -Wall -Wextra -Wpedantic)

add_executable(lab2 main.cpp)
target_link_libraries(lab2 PRIVATE string_lib)
target_compile_options(lab2 PRIVATE -Wall -Wextra -Wpedantic)

include(CTest)
if(BUILD_TESTING)
    include(FetchContent)
    if(POLICY CMP0135)
        cmake_policy(SET CMP0135 NEW)
    endif()
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
        URL_HASH SHA256=8ad598c73ad796e0d8280b082cebd82a630d73e73cd3c70057938a6501bba5d7
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(test_string tests/test_string.cpp)
    target_link_libraries(test_string PRIVATE string_lib GTest::gtest GTest::gtest_main)
    target_compile_options(test_string PRIVATE -Wall -Wextra -Wpedantic)

    include(GoogleTest)
    gtest_discover_tests(test_string)
endif()
