# 设置 CMake 最低版本要求
cmake_minimum_required(VERSION 3.20)

# 定义项目名称、版本和使用的语言
project(MathModule VERSION 1.0.0 LANGUAGES CXX)

# 创建静态库目标
add_library(math_module STATIC
    math.cpp
)

# 设置库的头文件目录
# PUBLIC 表示该目录对库本身和使用该库的目标都可见
target_include_directories(math_module PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 设置库的 C++ 标准为 C++20
target_compile_features(math_module PUBLIC cxx_std_20)

# 输出配置状态信息
message(STATUS "  - Math module configured")
