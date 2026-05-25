cmake_minimum_required(VERSION 3.20)

project(TestingExample VERSION 1.0.0 LANGUAGES CXX)

add_library(testing_math STATIC
    math.cpp
)

target_include_directories(testing_math PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(testing_math PUBLIC cxx_std_20)

add_executable(testing_example main.cpp)

target_link_libraries(testing_example PRIVATE testing_math)

add_executable(test_add test_add.cpp)
target_link_libraries(test_add PRIVATE testing_math)
add_test(NAME test_add COMMAND test_add)

add_executable(test_multiply test_multiply.cpp)
target_link_libraries(test_multiply PRIVATE testing_math)
add_test(NAME test_multiply COMMAND test_multiply)

add_executable(test_factorial test_factorial.cpp)
target_link_libraries(test_factorial PRIVATE testing_math)
add_test(NAME test_factorial COMMAND test_factorial)

message(STATUS "07-testing: Testing example configured")
message(STATUS "  Tests: test_add, test_multiply, test_factorial")
message(STATUS "  Run tests: ctest --output-on-failure")
