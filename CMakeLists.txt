cmake_minimum_required(VERSION 3.11)
project(localisation-cpp)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

option(localisation_ENABLE_INSTALL "Enable install rule" ON)
set(ABSL_ENABLE_INSTALL ${localisation_ENABLE_INSTALL})

FetchContent_Declare(
        open-lib
        GIT_REPOSITORY https://github.com/Andrewkoro105/open-lib.git
        GIT_TAG master
        FIND_PACKAGE_ARGS NAMES open-lib
)

FetchContent_Declare(
        abseil
        GIT_REPOSITORY https://github.com/abseil/abseil-cpp
        GIT_TAG         "20240116.0"
        FIND_PACKAGE_ARGS NAMES absl
)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.13.0
        FIND_PACKAGE_ARGS NAMES GTest
)

FetchContent_Declare(
        LocalizationZip
        GIT_REPOSITORY https://github.com/Andrewkoro105/Localization_Zip.git
        GIT_TAG        master
        FIND_PACKAGE_ARGS NAMES LocalizationZip
)

FetchContent_Declare(
        option_result_cpp
        GIT_REPOSITORY https://github.com/Hedgehogo/option-result-cpp.git
        GIT_TAG        main
        FIND_PACKAGE_ARGS NAMES option_result_cpp
)

FetchContent_MakeAvailable(open-lib abseil googletest LocalizationZip option_result_cpp)

add_library(${PROJECT_NAME} lib/localisation/system.cpp)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>
        $<INSTALL_INTERFACE:include>
        )
target_link_libraries(${PROJECT_NAME} PUBLIC open-lib absl::flat_hash_map option_result_cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER lib/localisation/system.hpp)

add_executable(${PROJECT_NAME}_ex main.cpp)
target_link_libraries(${PROJECT_NAME}_ex PUBLIC ${PROJECT_NAME})

add_executable(${PROJECT_NAME}_test test/test.cpp)
target_link_libraries(${PROJECT_NAME}_test PUBLIC ${PROJECT_NAME} gtest gtest_main)
add_dependencies(${PROJECT_NAME}_test LocalizationZip)

if (localisation_ENABLE_INSTALL)
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
endif ()