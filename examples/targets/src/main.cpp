#include <iostream>
#include "math_lib.h"
#include "string_utils.h"

int main() {
    std::cout << "====================================\n";
    std::cout << "  03 - Targets in CMake\n";
    std::cout << "====================================\n";
    std::cout << "\n";

    std::cout << "Key concepts demonstrated:\n";
    std::cout << "1. add_library(name STATIC ...) - Static library\n";
    std::cout << "   - Embedded into executable at link time\n";
    std::cout << "   - File: libmath_lib.a / math_lib.lib\n";
    std::cout << "\n";
    std::cout << "2. add_library(name SHARED ...) - Shared library\n";
    std::cout << "   - Loaded at runtime, must be distributed with app\n";
    std::cout << "   - File: libstring_utils.so / string_utils.dll\n";
    std::cout << "\n";
    std::cout << "3. Visibility keywords:\n";
    std::cout << "   PUBLIC: this target + consumers\n";
    std::cout << "   PRIVATE: this target only\n";
    std::cout << "   INTERFACE: consumers only\n";
    std::cout << "\n";

    std::cout << "Testing math_lib (STATIC):\n";
    std::cout << "  5 + 3 = " << math::add(5, 3) << "\n";
    std::cout << "  5 * 3 = " << math::multiply(5, 3) << "\n";
    std::cout << "  5! = " << math::factorial(5) << "\n";
    std::cout << "\n";

    std::cout << "Testing string_utils (SHARED):\n";
    std::cout << "  reverse(\"hello\") = " << string_utils::reverse("hello") << "\n";
    std::cout << "  to_upper(\"world\") = " << string_utils::to_upper("world") << "\n";
    std::cout << "  count_words(\"hello world foo\") = " << string_utils::count_words("hello world foo") << "\n";
    std::cout << "\n";

    std::cout << "STATIC vs SHARED:\n";
    std::cout << "  STATIC: larger binary, no runtime dependency\n";
    std::cout << "  SHARED: smaller binary, needs .dll/.so at runtime\n";
    std::cout << "====================================\n";

    return 0;
}
