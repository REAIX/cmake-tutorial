# 设置 CMake 最低版本要求
cmake_minimum_required(VERSION 3.20)
# 参考文档：docs/02-CMake常用命令参考.md 和 docs/05-CMake进阶技巧.md

# 定义项目名称、版本和使用的语言
project(HeaderOnlyExample VERSION 1.0.0 LANGUAGES CXX)

# 选项：是否启用调试输出（演示 #cmakedefine）
option(ENABLE_DEBUG_OUTPUT "Enable debug output in header-only example" OFF)

# 使用 configure_file 从模板生成配置头文件
# @VAR@ 会被替换为 CMake 变量的值
# #cmakedefine 会根据变量是否定义生成 #define 或 /* #undef */
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
    @ONLY
)

# 创建 INTERFACE 库（Header-Only 库）
# INTERFACE 库没有源文件，仅传播属性给链接它的目标
add_library(format_lib INTERFACE)

# 设置头文件包含路径（INTERFACE 可见性：仅消费者可见）
target_include_directories(format_lib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

# 传播 C++ 标准要求给消费者（INTERFACE 可见性）
target_compile_features(format_lib INTERFACE cxx_std_20)

# 创建可执行目标
add_executable(header_only_example main.cpp)

# 链接 Header-Only 库（format_lib 的 INTERFACE cxx_std_20 会自动传播）
target_link_libraries(header_only_example PRIVATE format_lib)

# 输出配置状态信息
message(STATUS "09-header-only: Header-only example configured")
message(STATUS "  Library: format_lib (INTERFACE / header-only)")
message(STATUS "  configure_file: config.h.in -> config.h")
message(STATUS "  ENABLE_DEBUG_OUTPUT: ${ENABLE_DEBUG_OUTPUT}")
