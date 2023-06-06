cmake_minimum_required(VERSION 3.23)
project(localisation)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
        OpenLib
        GIT_REPOSITORY https://github.com/Andrewkoro105/OpenLib.git
        GIT_TAG master
        FIND_PACKAGE_ARGS NAMES openLib
)

FetchContent_MakeAvailable(OpenLib)

add_library(${PROJECT_NAME} lib/system.cpp)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>
        $<INSTALL_INTERFACE:include>
        )
target_link_libraries(${PROJECT_NAME} PUBLIC openLib)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER lib/system.hpp)

add_executable(${PROJECT_NAME}_ex main.cpp)
target_link_libraries(${PROJECT_NAME}_ex PUBLIC ${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
        )

install(EXPORT ${PROJECT_NAME}
        FILE ${PROJECT_NAME}Config.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION lib/cmake/${PROJECT_NAME}
        )