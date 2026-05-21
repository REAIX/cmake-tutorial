# 设置 CMake 最低版本要求
cmake_minimum_required(VERSION 3.20)

# 定义项目名称、版本和使用的语言
project(BasicExample VERSION 1.0.0 LANGUAGES CXX)

# 创建可执行目标
add_executable(basic_example main.cpp)

# 设置 C++ 标准为 C++20
target_compile_features(basic_example PRIVATE cxx_std_20)

# 输出配置状态信息
message(STATUS "01-basic: Basic example configured")
