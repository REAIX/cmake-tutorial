cmake_minimum_required(VERSION 3.20)

project(AdvancedExample VERSION 1.0.0 LANGUAGES CXX)

set(GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GENERATED_DIR})

find_package(Git QUIET)

if(GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    message(STATUS "Git commit: ${GIT_COMMIT_HASH}")
endif()

if(NOT GIT_COMMIT_HASH)
    set(GIT_COMMIT_HASH "unknown")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

string(TIMESTAMP BUILD_TIMESTAMP "%Y-%m-%d %H:%M:%S")
message(STATUS "Build timestamp: ${BUILD_TIMESTAMP}")

add_custom_command(
    OUTPUT ${GENERATED_DIR}/build_info.hpp
    COMMAND ${CMAKE_COMMAND}
        -DOUTPUT_FILE="${GENERATED_DIR}/build_info.hpp"
        -DPROJECT_VERSION="${PROJECT_VERSION}"
        -DBUILD_TYPE="${CMAKE_BUILD_TYPE}"
        -DGIT_COMMIT_HASH="${GIT_COMMIT_HASH}"
        -DBUILD_TIMESTAMP="${BUILD_TIMESTAMP}"
        -P ${CMAKE_CURRENT_SOURCE_DIR}/generate_build_info.cmake
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/generate_build_info.cmake
    COMMENT "Generating build_info.hpp from CMake configuration"
)

add_custom_target(generate_build_info
    DEPENDS ${GENERATED_DIR}/build_info.hpp
)

add_executable(advanced_example main.cpp)

add_dependencies(advanced_example generate_build_info)

target_include_directories(advanced_example PRIVATE ${GENERATED_DIR})

target_compile_features(advanced_example PRIVATE cxx_std_20)

install(TARGETS advanced_example
    RUNTIME DESTINATION bin
)

set(CPACK_PACKAGE_NAME "AdvancedExample")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Advanced CMake Example")
set(CPACK_GENERATOR "ZIP")
include(CPack)

message(STATUS "05-advanced: Advanced example configured")
message(STATUS "  Code generation: build_info.hpp")
message(STATUS "  Git integration: ${GIT_FOUND}")
message(STATUS "  Git commit: ${GIT_COMMIT_HASH}")
message(STATUS "  Install targets: configured")
message(STATUS "  Packaging: enabled (CPack)")
