cmake_minimum_required(VERSION 3.14)
project(ARC_cache_project LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/headers)

option(ENABLE_SANITIZERS "Enable address sanitizer in Debug builds" ON)

function(setup_sanitizers target)
    if(ENABLE_SANITIZERS AND (CMAKE_BUILD_TYPE STREQUAL "Debug" OR NOT CMAKE_BUILD_TYPE))
        target_compile_options(${target} PRIVATE -fsanitize=address -fno-omit-frame-pointer)
        target_link_options(${target} PRIVATE -fsanitize=address)
    endif()
endfunction()

add_executable(arc_cache src/ARC_cache_main.cpp)
setup_sanitizers(arc_cache)

add_executable(tests src/test_main.cpp)
setup_sanitizers(tests)

set_source_files_properties(src/Perfect_cache_main.c PROPERTIES LANGUAGE CXX)
add_executable(perfect_cache src/Perfect_cache_main.c)
setup_sanitizers(perfect_cache)
