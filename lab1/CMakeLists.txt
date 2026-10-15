cmake_minimum_required(VERSION 3.16)

project(lab1_array LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    set(LAB_WARNINGS /W4)
else()
    set(LAB_WARNINGS -Wall -Wextra -Wpedantic)
endif()

add_library(array_ops STATIC array_ops.cpp)
target_include_directories(array_ops PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(array_ops PRIVATE ${LAB_WARNINGS})

add_executable(lab1 main.cpp)
target_link_libraries(lab1 PRIVATE array_ops)
target_compile_options(lab1 PRIVATE ${LAB_WARNINGS})

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

enable_testing()
add_executable(variant_tests test_variant.cpp)
target_link_libraries(variant_tests PRIVATE array_ops GTest::gtest_main)
target_compile_options(variant_tests PRIVATE ${LAB_WARNINGS})

include(GoogleTest)
gtest_discover_tests(variant_tests)