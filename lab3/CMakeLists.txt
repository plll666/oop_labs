cmake_minimum_required(VERSION 3.20)
project(oop_lab3_student_set LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    set(LAB_WARNINGS /W4)
else()
    set(LAB_WARNINGS -Wall -Wextra -Wpedantic)
endif()

option(ENABLE_ASAN "Build with AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
if(ENABLE_ASAN)
    if(MSVC)
        add_compile_options(/fsanitize=address /Zi)
    else()
        add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer -g)
        add_link_options(-fsanitize=address,undefined)
    endif()
endif()

add_library(student_lib STATIC student.cpp student_set.cpp)
target_include_directories(student_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(student_lib PRIVATE ${LAB_WARNINGS})

add_executable(lab3 main.cpp)
target_link_libraries(lab3 PRIVATE student_lib)
target_compile_options(lab3 PRIVATE ${LAB_WARNINGS})

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

    add_executable(lab_03_tests tests/test_student_set.cpp)
    target_link_libraries(lab_03_tests PRIVATE student_lib GTest::gtest_main)
    target_compile_options(lab_03_tests PRIVATE ${LAB_WARNINGS})

    include(GoogleTest)
    gtest_discover_tests(lab_03_tests)
endif()
