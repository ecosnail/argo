cmake_minimum_required (VERSION 3.8)

project (argo)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_library (argo
    exceptions.cpp
    global_parser.cpp
    parser.cpp
)
target_include_directories (argo PUBLIC include)

add_subdirectory (examples)

enable_testing ()
add_subdirectory (tests)
