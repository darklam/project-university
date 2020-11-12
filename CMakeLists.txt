cmake_minimum_required(VERSION 3.0.0)
project(project VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(project src/main.cpp src/CameraDTO.cpp src/Clique.cpp src/CSV.cpp src/FileSystem.cpp src/JSON.cpp src/Set.cpp src/Utils.cpp)

target_include_directories(project PRIVATE include)
set_property(TARGET project PROPERTY CXX_STANDARD 17)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
