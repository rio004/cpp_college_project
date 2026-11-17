cmake_minimum_required(VERSION 3.14)

project(EsportsScout CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------------------
# Google Test (Topic 11) - downloaded the first time cmake is run.
# ------------------------------------------------------------------
include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ------------------------------------------------------------------
# The application (menu + all twelve topics)
# ------------------------------------------------------------------
add_executable(
    main_app
    main.cpp
    main_functions.cpp
)

# ------------------------------------------------------------------
# The unit tests (only main_functions.cpp is under test)
# ------------------------------------------------------------------
add_executable(
    scout_tests
    function_tests.cpp
    main_functions.cpp
)

target_link_libraries(
    scout_tests
    PRIVATE
    gtest_main
)

enable_testing()
add_test(NAME unit_tests COMMAND scout_tests)
