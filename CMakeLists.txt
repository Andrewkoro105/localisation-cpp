cmake_minimum_required(VERSION 3.23)
project(localisation)

set(CMAKE_CXX_STANDARD 17)

file(COPY lib/system.hpp DESTINATION ${CMAKE_CURRENT_LIST_DIR}/include/localisation)
set(localisation_INCLUDE_DIRS include)

add_library(${PROJECT_NAME} lib/system.cpp)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER lib/system.hpp)

add_executable(${PROJECT_NAME}_ex main.cpp)
target_link_libraries(${PROJECT_NAME}_ex PUBLIC ${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
        )

install(EXPORT ${PROJECT_NAME}
        FILE ${PROJECT_NAME}-config.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION lib/cmake/${PROJECT_NAME}
        )