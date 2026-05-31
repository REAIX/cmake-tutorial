#include <iostream>
#include "math_lib.h"

int main() {
    std::cout << "====================================\n";
    std::cout << "  07 - Testing with CMake/CTest\n";
    std::cout << "====================================\n";
    std::cout << "\n";

    std::cout << "Key concepts demonstrated:\n";
    std::cout << "1. enable_testing() - Enable CTest support\n";
    std::cout << "2. add_test() - Register test executables\n";
    std::cout << "3. ctest command - Run all tests\n";
    std::cout << "4. Test isolation - Each test is independent\n";
    std::cout << "\n";

    std::cout << "Testing math library:\n";
    std::cout << "  add(2, 3) = " << math::add(2, 3) << "\n";
    std::cout << "  multiply(4, 5) = " << math::multiply(4, 5) << "\n";
    std::cout << "  factorial(6) = " << math::factorial(6) << "\n";
    std::cout << "\n";

    std::cout << "Run tests with:\n";
    std::cout << "  ctest --output-on-failure\n";
    std::cout << "  ctest --parallel 4\n";
    std::cout << "  ctest -R test_add\n";
    std::cout << "  ctest -T Test\n";
    std::cout << "====================================\n";

    return 0;
}
