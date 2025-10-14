cmake_minimum_required(VERSION 3.15)
project(CacheTests)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -O2")
endif()

# Include headers directory (correct path)
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../headers
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# ARC Cache Tests
add_executable(arc_tests
    ARC_cache_tests.cpp
)

# Perfect Cache Tests  
add_executable(ideal_tests
    Perfect_cache_tests.cpp
)

# Create custom targets for running tests
add_custom_target(run_arc_tests
    COMMAND ./arc_tests
    DEPENDS arc_tests
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running ARC cache tests"
)

add_custom_target(run_ideal_tests
    COMMAND ./ideal_tests
    DEPENDS ideal_tests
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running Ideal cache tests"
)

add_custom_target(run_all_tests
    COMMAND echo "Running ARC cache tests..." && ./arc_tests && echo "" && echo "Running Ideal cache tests..." && ./ideal_tests
    DEPENDS arc_tests ideal_tests
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running all cache tests"
)

# Print configuration info
message(STATUS "Project configured successfully")
message(STATUS "Header files directory: ${CMAKE_CURRENT_SOURCE_DIR}/../headers")
message(STATUS "Source files:")
message(STATUS "  - ${CMAKE_CURRENT_SOURCE_DIR}/ARC_cache_tests.cpp")
message(STATUS "  - ${CMAKE_CURRENT_SOURCE_DIR}/Perfect_cache_tests.cpp")
