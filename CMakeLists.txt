cmake_minimum_required(VERSION 3.0.0)
project(project VERSION 0.1.0)
find_package(Threads)

SET(CMAKE_CXX_FLAGS_PRODUCTION "-O3 -g -march=native -mtune=native -DNDEBUG -w")

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")
add_executable(project ${SOURCES})

target_include_directories(project PRIVATE include)

set_property(TARGET project PROPERTY CXX_STANDARD 17)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

add_subdirectory(test)
target_link_libraries(project ${CMAKE_THREAD_LIBS_INIT})
include(CPack)
