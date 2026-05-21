// 基础 CMake 示例程序
// 演示最简单的 CMake 项目结构

#include <iostream>

// 主函数：程序入口点
int main() {
    // 输出标题和分隔线
    std::cout << "====================================\n";
    std::cout << "  01 - Basic CMake Example\n";
    std::cout << "====================================\n";
    
    // 项目简介
    std::cout << "This is the simplest CMake project.\n";
    std::cout << "\n";
    
    // 展示关键概念
    std::cout << "Key concepts:\n";
    std::cout << "1. cmake_minimum_required(VERSION 3.20)\n";
    std::cout << "   - Sets the minimum CMake version\n";
    std::cout << "2. project(BasicExample VERSION 1.0.0)\n";
    std::cout << "   - Defines project name and version\n";
    std::cout << "3. add_executable(basic_example main.cpp)\n";
    std::cout << "   - Creates an executable target\n";
    std::cout << "\n";
    
    // 构建步骤说明
    std::cout << "Build steps:\n";
    std::cout << "  mkdir build && cd build\n";
    std::cout << "  cmake ..\n";
    std::cout << "  cmake --build .\n";
    std::cout << "====================================\n";
    
    // 程序正常退出
    return 0;
}
