cmake_minimum_required(VERSION 3.20)
project(GWBasic64)

set(CMAKE_CXX_STANDARD 17)

# Core interpreter source files
file(GLOB_RECURSE CORE_SRC_FILES
    gw_basic/*.cpp
)

# Core library (NOT an executable)
add_library(gw_basic_core STATIC ${CORE_SRC_FILES})

# Include headers
target_include_directories(gw_basic_core PUBLIC
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

# Main executable
add_executable(gw_basic_64 main.cpp)
target_link_libraries(gw_basic_64 PRIVATE gw_basic_core)

# Enable testing and add test subdirectory
enable_testing()
add_subdirectory(tests)
#set_target_properties(run_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)
