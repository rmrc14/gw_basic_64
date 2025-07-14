cmake_minimum_required(VERSION 3.20)
project(GWBasic64 )

set(CMAKE_CXX_STANDARD 17)

# Include subdirectories (headers are in these folders)
include_directories(
    gw_basic
    gw_basic/ProgramInterface
    gw_basic/Storage
    gw_basic/Lexer
    gw_basic/Parser
    gw_basic/Runtime
    gw_basic/IO
    gw_basic/System
    gw_basic/Errors
   
)

# Recursively find all source files
file(GLOB_RECURSE SRC_FILES gw_basic/*.cpp)

# Main executable
add_executable(gw_basic_64 main.cpp ${SRC_FILES})

# Unit testing
enable_testing()
add_subdirectory(tests)
