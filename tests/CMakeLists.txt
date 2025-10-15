cmake_minimum_required(VERSION 3.14)
project(ARC_cache_project LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(../headers)

enable_testing()

add_executable(arc_cache_unit_tests ARC_cache_tests.cpp)
target_include_directories(arc_cache_unit_tests PRIVATE ../headers)
add_test(NAME ARC_CACHE_TESTS COMMAND arc_cache_unit_tests)

add_executable(perfect_cache_unit_tests Perfect_cache_tests.cpp)
target_include_directories(perfect_cache_unit_tests PRIVATE ../headers)
add_test(NAME PERFECT_CACHE_TESTS COMMAND perfect_cache_unit_tests)
