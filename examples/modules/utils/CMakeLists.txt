# 设置 CMake 最低版本要求
cmake_minimum_required(VERSION 3.20)

# 创建静态库目标
add_library(utils_module STATIC
    utils.cpp
)

# 设置库的头文件目录
# PUBLIC 表示该目录对库本身和使用该库的目标都可见
target_include_directories(utils_module PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 设置库的 C++ 标准为 C++20
target_compile_features(utils_module PUBLIC cxx_std_20)

# 输出配置状态信息
message(STATUS "  - Utils module configured")
