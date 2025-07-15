cmake_minimum_required(VERSION 3.16)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

FetchContent_MakeAvailable(googletest)

file(GLOB TEST_SOURCES *.cpp)

add_executable(run_tests ${TEST_SOURCES})
target_link_libraries(run_tests PRIVATE gtest gtest_main gw_basic_core)

enable_testing()
include(GoogleTest)
gtest_discover_tests(run_tests)
