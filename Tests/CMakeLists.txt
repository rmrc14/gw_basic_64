cmake_minimum_required(VERSION 3.16)

include(FetchContent)

# Download and configure GoogleTest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

FetchContent_MakeAvailable(googletest)

# Add test files
file(GLOB TEST_SOURCES *.cpp)

add_executable(run_tests ${TEST_SOURCES})

# Link to the downloaded GTest build
target_link_libraries(run_tests gtest gtest_main)

enable_testing()
include(GoogleTest)
gtest_discover_tests(run_tests)
