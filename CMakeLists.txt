cmake_minimum_required(VERSION 3.14)
project(ARC_cache_project LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/headers)

option(ENABLE_SANITIZERS "Enable address sanitizer in Debug builds" ON)
option(ENABLE_TIME_MEASURE "Enable time measurement code" OFF)

function(setup_sanitizers target)
    if(ENABLE_SANITIZERS AND (CMAKE_BUILD_TYPE STREQUAL "Debug" OR NOT CMAKE_BUILD_TYPE))
        target_compile_options(${target} PRIVATE -fsanitize=address -fno-omit-frame-pointer)
        target_link_options(${target} PRIVATE -fsanitize=address)
    endif()
endfunction()

function(setup_time_measure target)
    if(ENABLE_TIME_MEASURE)
        target_compile_definitions(${target} PRIVATE TIME_MEASURE)
    endif()
endfunction()

add_executable(arc_cache src/ARC_cache_main.cpp)
setup_sanitizers(arc_cache)
setup_time_measure(arc_cache)

add_executable(tests src/test_main.cpp)
setup_sanitizers(tests)
setup_time_measure(tests)

set_source_files_properties(src/Perfect_cache_main.cpp PROPERTIES LANGUAGE CXX)
add_executable(perfect_cache src/Perfect_cache_main.cpp)
setup_sanitizers(perfect_cache)
setup_time_measure(perfect_cache)
