// CMake 变量示例程序
// 演示如何在 CMake 中定义和使用变量

#include <iostream>

// 主函数：程序入口点
int main() {
    // 输出标题和分隔线
    std::cout << "====================================\n";
    std::cout << "  02 - Variables in CMake\n";
    std::cout << "====================================\n";
    std::cout << "\n";
    
    // 展示关键概念
    std::cout << "Key concepts demonstrated:\n";
    std::cout << "1. set() - Define variables\n";
    std::cout << "   set(MY_VARIABLE \"Hello CMake\")\n";
    std::cout << "\n";
    std::cout << "2. Lists in CMake\n";
    std::cout << "   set(MY_LIST item1 item2 item3)\n";
    std::cout << "\n";
    std::cout << "3. Cache variables\n";
    std::cout << "   set(VAR value CACHE TYPE \"description\")\n";
    std::cout << "   - Persists between CMake runs\n";
    std::cout << "   - Can be set via cmake -DVAR=value\n";
    std::cout << "\n";
    std::cout << "4. Conditionals\n";
    std::cout << "   if() / elseif() / else() / endif()\n";
    std::cout << "\n";
    std::cout << "5. Passing variables to C++\n";
    std::cout << "   target_compile_definitions(target PRIVATE VAR=\"value\")\n";
    std::cout << "\n";
    
    // 检查特性 A 是否启用（通过 CMake 的 target_compile_definitions 定义）
#ifdef FEATURE_A_ENABLED
    std::cout << "Feature A: ENABLED\n";
#else
    std::cout << "Feature A: DISABLED\n";
#endif

    // 检查特性 B 是否启用
#ifdef FEATURE_B_ENABLED
    std::cout << "Feature B: ENABLED\n";
#else
    std::cout << "Feature B: DISABLED\n";
#endif

    // 输出从 CMake 传递过来的变量
    std::cout << "\n";
    std::cout << "Application version: " << APP_VERSION << "\n";
    std::cout << "My path: " << MY_PATH << "\n";
    std::cout << "\n";
    
    // 提示用户尝试的 CMake 选项
    std::cout << "Try these cmake options:\n";
    std::cout << "  cmake -DENABLE_FEATURE_A=OFF ..\n";
    std::cout << "  cmake -DENABLE_FEATURE_B=ON ..\n";
    std::cout << "  cmake -DCMAKE_BUILD_TYPE=Debug ..\n";
    std::cout << "====================================\n";
    
    // 程序正常退出
    return 0;
}
