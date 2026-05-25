cmake_minimum_required(VERSION 3.20)

project(TargetsExample VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(math_lib STATIC
    lib/math.cpp
)

target_include_directories(math_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

target_compile_features(math_lib PUBLIC cxx_std_20)

add_library(string_utils SHARED
    lib/string_utils.cpp
)

target_include_directories(string_utils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

target_compile_features(string_utils PUBLIC cxx_std_20)

add_executable(targets_example src/main.cpp)

target_link_libraries(targets_example PRIVATE math_lib string_utils)

message(STATUS "03-targets: Targets example configured")
message(STATUS "  Libraries: math_lib (STATIC), string_utils (SHARED)")
message(STATUS "  Executable: targets_example")
