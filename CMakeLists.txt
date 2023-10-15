cmake_minimum_required(VERSION 3.23)
project(localisation-cpp)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
        open-lib
        GIT_REPOSITORY https://github.com/Andrewkoro105/open-lib.git
        GIT_TAG master
        FIND_PACKAGE_ARGS NAMES open-lib
)

FetchContent_MakeAvailable(open-lib)

add_library(${PROJECT_NAME} lib/localisation/system.cpp)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>
        $<INSTALL_INTERFACE:include>
        )
target_link_libraries(${PROJECT_NAME} PUBLIC open-lib)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER lib/localisation/system.hpp)

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