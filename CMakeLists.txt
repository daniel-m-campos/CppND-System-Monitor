cmake_minimum_required(VERSION 2.6)
project(monitor)

set(CMAKE_CXX_STANDARD 17)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

set(BUILD_TESTS NO CACHE BOOL "Build test target")
if (BUILD_TESTS)
    add_library(
            linux_parser
            src/linux_parser.cpp
            src/processor.cpp
            src/process.cpp
            src/system.cpp
            src/format.cpp
    )
    add_subdirectory(tests)
endif ()

add_executable(monitor ${SOURCES})

set_property(TARGET monitor PROPERTY CXX_STANDARD 17)
target_link_libraries(monitor ${CURSES_LIBRARIES})
# TODO: Run -Werror in CI.
target_compile_options(monitor PRIVATE -Wall -Wextra)
