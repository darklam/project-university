cmake_minimum_required(VERSION 3.0.0)
project(project VERSION 0.1.0)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")
add_executable(project ${SOURCES})

target_include_directories(project PRIVATE include)

set_property(TARGET project PROPERTY CXX_STANDARD 17)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

add_subdirectory(test)

include(CPack)
