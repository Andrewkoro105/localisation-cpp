cmake_minimum_required(VERSION 3.23)
project(localization)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} SHARED lib/system.cpp lib/system.hpp)

install(FILES lib/system.hpp DESTINATION loc)

add_executable(${PROJECT_NAME}_ex main.cpp)

target_link_libraries(${PROJECT_NAME}_ex PUBLIC ${PROJECT_NAME})
