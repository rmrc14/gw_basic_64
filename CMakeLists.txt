cmake_minimum_required(VERSION 3.20)
project(GWBasic64)

set(CMAKE_CXX_STANDARD 17)

# Core interpreter source files (excluding platform system interface)
file(GLOB_RECURSE CORE_SRC_FILES
    gw_basic/*.cpp
)
list(REMOVE_ITEM CORE_SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/gw_basic/System/SystemInterface_Windows.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gw_basic/System/SystemInterface_MiniOS.cpp
)

# Choose platform-specific system interface
if(WIN32)
    set(PLATFORM_SYSTEM_SRC gw_basic/System/SystemInterface_Windows.cpp)
else()
    set(PLATFORM_SYSTEM_SRC gw_basic/System/SystemInterface_MiniOS.cpp)
endif()

# Create static library
add_library(gw_basic_core STATIC
    ${CORE_SRC_FILES}
    ${PLATFORM_SYSTEM_SRC}
)

# Public includes
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

# Enable tests
enable_testing()
add_subdirectory(tests)


#target_include_directories(run_tests PRIVATE
#    ${PROJECT_SOURCE_DIR}/gw_basic
#)
