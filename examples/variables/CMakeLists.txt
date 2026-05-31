# 设置 CMake 最低版本要求
cmake_minimum_required(VERSION 3.20)
# 参考文档：docs/01-CMake基础概念.md（变量部分）

# 定义项目名称、版本和使用的语言
project(VariablesExample VERSION 1.0.0 LANGUAGES CXX)

# 基本变量定义
set(MY_VARIABLE "Hello CMake")
set(MY_LIST item1 item2 item3)
set(MY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/data)

# 版本号变量
set(APP_VERSION_MAJOR 1)
set(APP_VERSION_MINOR 0)
set(APP_VERSION_PATCH 0)
set(APP_VERSION "${APP_VERSION_MAJOR}.${APP_VERSION_MINOR}.${APP_VERSION_PATCH}")

# 缓存变量 - 可通过 cmake -D 选项设置
set(ENABLE_FEATURE_A ON CACHE BOOL "Enable feature A")
set(ENABLE_FEATURE_B OFF CACHE BOOL "Enable feature B")

# 创建可执行目标
add_executable(variables_example main.cpp)

# 设置 C++ 标准为 C++20
target_compile_features(variables_example PRIVATE cxx_std_20)

# 条件判断：特性 A
if(ENABLE_FEATURE_A)
    message(STATUS "Feature A is enabled")
    target_compile_definitions(variables_example PRIVATE FEATURE_A_ENABLED)
endif()

# 条件判断：特性 B
if(ENABLE_FEATURE_B)
    message(STATUS "Feature B is enabled")
    target_compile_definitions(variables_example PRIVATE FEATURE_B_ENABLED)
endif()

# 条件判断：构建类型
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug build mode")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Release build mode")
else()
    message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
endif()

# 将变量传递给 C++ 代码
target_compile_definitions(variables_example PRIVATE
    APP_VERSION="${APP_VERSION}"
    MY_PATH="${MY_PATH}"
)

# 输出配置状态信息
message(STATUS "02-variables: Variables example configured")
message(STATUS "  MY_VARIABLE = ${MY_VARIABLE}")
message(STATUS "  MY_LIST = ${MY_LIST}")
message(STATUS "  APP_VERSION = ${APP_VERSION}")
