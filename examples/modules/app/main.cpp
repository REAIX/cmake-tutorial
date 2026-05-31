// CMake 模块化项目示例程序
// 演示如何使用 add_subdirectory 构建多模块项目

#include <iostream>
#include "math_lib.h"
#include "utils.h"

// 主函数：程序入口点
int main() {
    // 输出标题和分隔线
    std::cout << "====================================\n";
    std::cout << "  04 - Modular Projects in CMake\n";
    std::cout << "====================================\n";
    std::cout << "\n";
    
    // 展示关键概念
    std::cout << "Key concepts demonstrated:\n";
    std::cout << "1. add_subdirectory() - Include subdirectories\n";
    std::cout << "   - Each subdirectory has its own CMakeLists.txt\n";
    std::cout << "   - Builds a modular project structure\n";
    std::cout << "\n";
    std::cout << "2. Project structure:\n";
    std::cout << "   04-modules/\n";
    std::cout << "   ├── CMakeLists.txt (root)\n";
    std::cout << "   ├── app/main.cpp\n";
    std::cout << "   ├── math/\n";
    std::cout << "   │   ├── CMakeLists.txt\n";
    std::cout << "   │   ├── math.h\n";
    std::cout << "   │   └── math.cpp\n";
    std::cout << "   └── utils/\n";
    std::cout << "       ├── CMakeLists.txt\n";
    std::cout << "       ├── utils.h\n";
    std::cout << "       └── utils.cpp\n";
    std::cout << "\n";
    std::cout << "3. Benefits:\n";
    std::cout << "   - Clear separation of concerns\n";
    std::cout << "   - Independent module configuration\n";
    std::cout << "   - Reusable components\n";
    std::cout << "\n";
    
    // 测试数学模块功能
    std::cout << "Testing math module:\n";
    std::cout << "  square(5.0) = " << math_module::square(5.0) << "\n";
    std::cout << "  power(2, 8) = " << math_module::power(2, 8) << "\n";
    std::cout << "  is_prime(17) = " << (math_module::is_prime(17) ? "true" : "false") << "\n";
    std::cout << "\n";
    
    // 测试工具模块功能
    std::cout << "Testing utils module:\n";
    std::cout << "  to_upper(\"hello\") = " << utils_module::to_upper("hello") << "\n";
    std::cout << "  to_lower(\"WORLD\") = " << utils_module::to_lower("WORLD") << "\n";
    auto parts = utils_module::split("a,b,c,d", ',');
    std::cout << "  split(\"a,b,c,d\") = [";
    for (size_t i = 0; i < parts.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << parts[i];
    }
    std::cout << "]\n";
    std::cout << "\n";
    std::cout << "====================================\n";
    
    // 程序正常退出
    return 0;
}
