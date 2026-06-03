cmake_minimum_required(VERSION 3.20)
# 参考文档：docs/03-CMake最佳实践.md（测试部分）

project(TestingExample VERSION 1.0.0 LANGUAGES CXX)

enable_testing()

add_library(testing_math STATIC
    math.cpp
)

target_include_directories(testing_math PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(testing_math PUBLIC cxx_std_20)

add_executable(testing_example main.cpp)

target_link_libraries(testing_example PRIVATE testing_math)

# ============================================================
# 简单测试方式：使用 assert 宏编写测试
# 优点：无需额外依赖，配置简单，适合小型项目
# 缺点：断言失败时信息有限，缺少测试发现/过滤/报告等高级功能
# ============================================================
add_executable(test_add test_add.cpp)
target_link_libraries(test_add PRIVATE testing_math)
add_test(NAME test_add COMMAND test_add)

add_executable(test_multiply test_multiply.cpp)
target_link_libraries(test_multiply PRIVATE testing_math)
add_test(NAME test_multiply COMMAND test_multiply)

add_executable(test_factorial test_factorial.cpp)
target_link_libraries(test_factorial PRIVATE testing_math)
add_test(NAME test_factorial COMMAND test_factorial)

# ============================================================
# GTest 测试方式：使用 GoogleTest 框架编写测试
# 优点：丰富的断言宏、测试发现与过滤、XML 报告、测试夹具等
# 缺点：引入外部依赖，配置稍复杂
# 启用方式：cmake -DUSE_GTEST=ON ..
# ============================================================
option(USE_GTEST "启用 GoogleTest 框架测试（默认关闭）" OFF)

if(USE_GTEST)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
    )
    # 防止在 Windows 上覆盖编译器/链接器设置
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(test_gtest test_gtest.cpp)
    target_link_libraries(test_gtest PRIVATE testing_math GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(test_gtest)

    message(STATUS "07-testing: GTest-based tests ENABLED")
    message(STATUS "  GTest tests will be discovered automatically")
else()
    message(STATUS "07-testing: GTest-based tests DISABLED (use -DUSE_GTEST=ON to enable)")
endif()

message(STATUS "07-testing: Testing example configured")
message(STATUS "  Simple tests: test_add, test_multiply, test_factorial")
message(STATUS "  Run tests: ctest --output-on-failure")
