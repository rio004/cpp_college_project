cmake_minimum_required(VERSION 3.14)
project(Lab1 CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    main_app
    main.cpp
    main_functions.cpp
)

# Not called run_tests: that name clashes with the RUN_TESTS project CMake
# generates on Windows, and the build then fails with ctest error 8
add_executable(
    unit_tests
    function_tests.cpp
    main_functions.cpp
)

target_link_libraries(
    unit_tests
    PRIVATE
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests)
