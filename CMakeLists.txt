cmake_minimum_required(VERSION 3.0.0)
project(project VERSION 0.1.0)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
add_executable(project ${SOURCES})

file(GLOB_RECURSE TEST_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
list(REMOVE_ITEM SOURCES "src/main.cpp")
add_executable(test test/test.cpp ${SOURCES})

target_include_directories(project PRIVATE include)
target_include_directories(test PRIVATE include)
set_property(TARGET project PROPERTY CXX_STANDARD 17)
set_property(TARGET test PROPERTY CXX_STANDARD 17)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
