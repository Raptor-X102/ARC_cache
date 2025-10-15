cmake_minimum_required(VERSION 3.14)
project(ARC_cache_project LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/headers)

add_executable(arc_cache src/ARC_cache_main.cpp)
add_executable(tests src/test_main.cpp)

set_source_files_properties(src/Perfect_cache_main.c PROPERTIES LANGUAGE CXX)
add_executable(perfect_cache src/Perfect_cache_main.c)
