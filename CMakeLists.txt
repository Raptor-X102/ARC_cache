cmake_minimum_required(VERSION 3.10)
project(arc_cache_project)

set(CMAKE_CXX_STANDARD 17)

set(HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/headers)

add_library(arc_cache_headers INTERFACE)

target_include_directories(arc_cache_headers INTERFACE
    $<BUILD_INTERFACE:${HEADER_DIR}>
    $<INSTALL_INTERFACE:include>
)

add_executable(arc_test src/test_main.cpp)

target_link_libraries(arc_test PRIVATE arc_cache_headers)
