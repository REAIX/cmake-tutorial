cmake_minimum_required(VERSION 3.20)

project(FetchContentExample VERSION 1.0.0 LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG        10.2.1
)

FetchContent_MakeAvailable(fmt)

add_executable(fetchcontent_example main.cpp)

target_compile_features(fetchcontent_example PRIVATE cxx_std_20)

target_link_libraries(fetchcontent_example PRIVATE fmt::fmt)

message(STATUS "08-fetchcontent: FetchContent example configured")
message(STATUS "  Dependency: fmt (fetched from GitHub)")
message(STATUS "  Build and run: cmake --build . && ./bin/fetchcontent_example")
