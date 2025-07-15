cmake_minimum_required(VERSION 3.20)

# Set output directory for all test binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Release)

# Fetch GoogleTest using FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

# Find all test source files
file(GLOB TEST_SOURCES *.cpp)

# Create the test executable
add_executable(run_tests ${TEST_SOURCES})
target_link_libraries(run_tests
  PRIVATE
    gtest
    gtest_main
    gw_basic_core
)

# Enable testing
enable_testing()

# Discover and register tests automatically
include(GoogleTest)
gtest_discover_tests(run_tests
  WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  DISCOVERY_TIMEOUT 30
)
