// Header-Only 库与 configure_file 示例程序
// 演示 INTERFACE 库、configure_file 和头文件库的使用

#include <iostream>
#include "format_lib.h"

// 主函数：程序入口点
int main() {
    // 输出标题和分隔线
    std::cout << "====================================\n";
    std::cout << "  09 - Header-Only Library & configure_file\n";
    std::cout << "====================================\n";
    std::cout << "\n";

    // 展示关键概念
    std::cout << "Key concepts demonstrated:\n";
    std::cout << "1. add_library(... INTERFACE)\n";
    std::cout << "   - Creates a header-only library target\n";
    std::cout << "   - No source files compiled into the library\n";
    std::cout << "\n";
    std::cout << "2. target_include_directories(... INTERFACE)\n";
    std::cout << "   - INTERFACE visibility: only consumers see the paths\n";
    std::cout << "   - Header-only libs have no 'self', so INTERFACE is required\n";
    std::cout << "\n";
    std::cout << "3. target_compile_features(... INTERFACE)\n";
    std::cout << "   - Propagates C++ standard requirement to consumers\n";
    std::cout << "\n";
    std::cout << "4. configure_file(config.h.in config.h)\n";
    std::cout << "   - Generates config.h from template at configure time\n";
    std::cout << "   - @VAR@ is replaced with CMake variable values\n";
    std::cout << "   - #cmakedefine becomes #define or /* #undef */\n";
    std::cout << "\n";

    // 演示 format_lib 功能
    std::cout << "------------------------------------\n";
    std::cout << "format_lib demo:\n";
    std::cout << "  format_number(42)  = " << format_lib::format_number(42) << "\n";
    std::cout << "  format_number(-7)  = " << format_lib::format_number(-7) << "\n";
    std::cout << "  format_hex(255)    = " << format_lib::format_hex(255) << "\n";
    std::cout << "  format_hex(65535)  = " << format_lib::format_hex(65535) << "\n";
    std::cout << "\n";

    // 输出 configure_file 生成的版本信息
    std::cout << "------------------------------------\n";
    std::cout << "configure_file output:\n";
    std::cout << "  HEADER_ONLY_EXAMPLE_VERSION = " << HEADER_ONLY_EXAMPLE_VERSION << "\n";

    // 检查 ENABLE_DEBUG_OUTPUT 是否通过 configure_file 定义
#ifdef ENABLE_DEBUG_OUTPUT
    std::cout << "  ENABLE_DEBUG_OUTPUT = defined\n";
#else
    std::cout << "  ENABLE_DEBUG_OUTPUT = not defined\n";
#endif

    std::cout << "\n";
    std::cout << "Try these cmake options:\n";
    std::cout << "  cmake -DENABLE_DEBUG_OUTPUT=ON ..\n";
    std::cout << "====================================\n";

    // 程序正常退出
    return 0;
}
