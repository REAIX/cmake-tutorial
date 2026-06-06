// CMake 编译器特定选项示例程序
// 演示如何检测和使用不同编译器的特性

#include <iostream>

// 检测编译器类型的宏（编译器内置）
// 这些宏由编译器自动定义
void print_compiler_info() {
    std::cout << "Compiler detection (built-in macros):\n";
    
    #if defined(_MSC_VER)
        std::cout << "  _MSC_VER = " << _MSC_VER << " (MSVC)\n";
    #endif
    
    #if defined(__GNUC__)
        std::cout << "  __GNUC__ = " << __GNUC__ << " (GCC major)\n";
        std::cout << "  __GNUC_MINOR__ = " << __GNUC_MINOR__ << "\n";
        std::cout << "  __GNUC_PATCHLEVEL__ = " << __GNUC_PATCHLEVEL__ << "\n";
    #endif
    
    #if defined(__clang__)
        std::cout << "  __clang_major__ = " << __clang_major__ << " (Clang major)\n";
        std::cout << "  __clang_minor__ = " << __clang_minor__ << "\n";
    #endif
    
    std::cout << "\n";
}

// 打印从 CMake 传递过来的编译器信息
void print_cmake_compiler_info() {
    std::cout << "Compiler info from CMake:\n";
    std::cout << "  Compiler: " << COMPILER_NAME << "\n";
    std::cout << "  Version: " << COMPILER_VERSION << "\n";
    std::cout << "\n";
}

// 根据 CMake 定义的宏判断编译器
void print_compiler_from_cmake() {
    std::cout << "Compiler detection (CMake definitions):\n";
    
    #ifdef COMPILER_MSVC
        std::cout << "  COMPILER_MSVC is defined\n";
    #endif
    
    #ifdef COMPILER_GCC
        std::cout << "  COMPILER_GCC is defined\n";
    #endif
    
    #ifdef COMPILER_CLANG
        std::cout << "  COMPILER_CLANG is defined\n";
    #endif
    
    std::cout << "\n";
}

// 演示编译器特定的代码
void demo_compiler_specific_code() {
    std::cout << "Compiler-specific code examples:\n";
    
    // 使用编译器内置宏进行条件编译
    #if defined(_MSC_VER)
        std::cout << "  MSVC-specific: Using #pragma warning\n";
        std::cout << "    #pragma warning(disable: 4996)  // Disable deprecated warning\n";
    #elif defined(__GNUC__)
        std::cout << "  GCC-specific: Using __attribute__\n";
        std::cout << "    __attribute__((unused)) int x = 0;\n";
    #elif defined(__clang__)
        std::cout << "  Clang-specific: Using __attribute__\n";
        std::cout << "    __attribute__((unused)) int x = 0;\n";
    #endif
    
    std::cout << "\n";
}

int main() {
    // 输出标题和分隔线
    std::cout << "====================================\n";
    std::cout << "  06 - Compiler-Specific Options\n";
    std::cout << "====================================\n";
    std::cout << "\n";
    
    // 展示关键概念
    std::cout << "Key concepts demonstrated:\n";
    std::cout << "1. CMAKE_CXX_COMPILER_ID - Identify compiler\n";
    std::cout << "   - MSVC, GNU, Clang, AppleClang, etc.\n";
    std::cout << "\n";
    std::cout << "2. Generator expressions:\n";
    std::cout << "   $<$<CXX_COMPILER_ID:MSVC>:options...>\n";
    std::cout << "   $<$<CXX_COMPILER_ID:GNU>:options...>\n";
    std::cout << "\n";
    std::cout << "3. target_compile_options() - Modern approach\n";
    std::cout << "   - Per-target settings\n";
    std::cout << "   - Generator expressions for flexibility\n";
    std::cout << "\n";
    std::cout << "4. Build type specific options:\n";
    std::cout << "   $<$<CONFIG:Debug>:debug-options...>\n";
    std::cout << "   $<$<CONFIG:Release>:release-options...>\n";
    std::cout << "\n";
    
    // 输出编译器信息
    print_compiler_info();
    print_cmake_compiler_info();
    print_compiler_from_cmake();
    demo_compiler_specific_code();
    
    // 常用编译器选项对比
    std::cout << "Common compiler options comparison:\n";
    std::cout << "\n";
    std::cout << "  Feature          | MSVC           | GCC/Clang\n";
    std::cout << "  -----------------|----------------|------------------\n";
    std::cout << "  Warning level    | /W1-/W4        | -Wall -Wextra\n";
    std::cout << "  Warnings as err  | /WX            | -Werror\n";
    std::cout << "  Optimization     | /O1 /O2 /Ox    | -O1 -O2 -O3 -Os\n";
    std::cout << "  Debug info       | /Zi            | -g\n";
    std::cout << "  Define macro     | /Dname=value   | -Dname=value\n";
    std::cout << "  Include path     | /Ipath         | -Ipath\n";
    std::cout << "  C++ standard     | /std:c++20     | -std=c++20\n";
    std::cout << "  Link library     | /DEFAULTLIB:lib| -llib\n";
    std::cout << "\n";
    
    // 构建命令示例
    std::cout << "Build commands:\n";
    std::cout << "  # Using MSVC (Visual Studio)\n";
    std::cout << "  cmake -G \"Visual Studio 17 2022\" ..\n";
    std::cout << "  cmake --build . --config Release\n";
    std::cout << "\n";
    std::cout << "  # Using GCC (MinGW on Windows)\n";
    std::cout << "  cmake -G \"MinGW Makefiles\" ..\n";
    std::cout << "  cmake --build .\n";
    std::cout << "\n";
    std::cout << "  # Using GCC (Linux)\n";
    std::cout << "  cmake -G \"Unix Makefiles\" ..\n";
    std::cout << "  cmake --build .\n";
    std::cout << "\n";
    std::cout << "  # Using Clang\n";
    std::cout << "  cmake -DCMAKE_CXX_COMPILER=clang++ ..\n";
    std::cout << "  cmake --build .\n";
    std::cout << "====================================\n";
    
    // 程序正常退出
    return 0;
}
