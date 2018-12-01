cmake_minimum_required (VERSION 3.8)

project (argo)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_library (argo INTERFACE)
target_include_directories (argo INTERFACE include)

add_subdirectory (examples)
