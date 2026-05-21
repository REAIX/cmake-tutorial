# 设置 CMake 最低版本要求
cmake_minimum_required(VERSION 3.20)

# 定义项目名称、版本和使用的语言
project(ModulesExample VERSION 1.0.0 LANGUAGES CXX)

# 添加子目录 - 每个子目录都有自己的 CMakeLists.txt
add_subdirectory(math)
add_subdirectory(utils)

# 创建可执行目标
add_executable(modules_example app/main.cpp)

# 链接库到可执行目标
# PRIVATE 表示依赖关系仅对当前目标可见
target_link_libraries(modules_example 
    PRIVATE 
        math_module
        utils_module
)

# 设置 C++ 标准为 C++20
target_compile_features(modules_example PRIVATE cxx_std_20)

# 输出配置状态信息
message(STATUS "04-modules: Modules example configured")
message(STATUS "  Subdirectories: math, utils")
message(STATUS "  Executable: modules_example")
