cmake_minimum_required (VERSION 3.8)

if (NOT CMAKE_MODULE_PATH)
    set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
endif ()

include (project)

ecosnail_project(
    C++ 17

    EXAMPLES
        basic
)
