cmake_minimum_required(VERSION 3.20)

project(cmake-tutorial VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type")
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 导出 compile_commands.json，供 clangd、VS Code IntelliSense 等工具使用
# 注意：仅对 Ninja 和 Unix Makefiles 生成器有效，Visual Studio 和 MinGW Makefiles 不支持
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_EXAMPLES "Build all examples" ON)
option(BUILD_FETCHCONTENT "Build FetchContent example (requires network)" OFF)

enable_testing()

if(BUILD_EXAMPLES)
    add_subdirectory(examples/basic)
    add_subdirectory(examples/variables)
    add_subdirectory(examples/targets)
    add_subdirectory(examples/modules)
    add_subdirectory(examples/advanced)
    add_subdirectory(examples/compiler-options)
    add_subdirectory(examples/testing)
    add_subdirectory(examples/header-only)
endif()

if(BUILD_FETCHCONTENT)
    add_subdirectory(examples/fetchcontent)
endif()

message(STATUS "====================================")
message(STATUS "cmake-tutorial Project Configuration")
message(STATUS "====================================")
message(STATUS "CMake version: ${CMAKE_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Build examples: ${BUILD_EXAMPLES}")
message(STATUS "====================================")
