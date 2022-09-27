cmake_minimum_required(VERSION 3.23)
project(localisation)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE src lib/loc/*.cpp lib/loc/*.hpp)

add_subdirectory(lib/submodule/libzip)

add_library(${PROJECT_NAME} ${src})

target_link_libraries(${PROJECT_NAME} PUBLIC zip)

add_executable(${PROJECT_NAME}_ex main.cpp)

target_link_libraries(${PROJECT_NAME}_ex PRIVATE ${PROJECT_NAME})