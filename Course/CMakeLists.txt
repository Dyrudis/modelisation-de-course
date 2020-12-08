cmake_minimum_required (VERSION 3.9)
project(Course)

set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )

if(MSVC)
    foreach(CompilerFlag ${CompilerFlags})
        string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach()
    set(SFML_STATIC_LIBRARIES TRUE)
    set(SFML_DIR "C:/SFML-2.5.1_windows_VS15.6_static/lib/cmake/SFML")
endif()

SET(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

set(TEST_SRCS "main.cpp" "course.cpp" "course.h"  )

add_executable (Course ${TEST_SRCS})
target_link_libraries(Course sfml-graphics sfml-window)