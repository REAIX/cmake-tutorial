#include <iostream>
#include "build_info.hpp"

int main() {
    std::cout << "====================================\n";
    std::cout << "  05 - Advanced CMake Features\n";
    std::cout << "====================================\n";
    std::cout << "\n";

    std::cout << "Key concepts demonstrated:\n";
    std::cout << "1. add_custom_command() - Generate code at build time\n";
    std::cout << "   - Uses a .cmake script for portable generation\n";
    std::cout << "   - DEPENDS ensures regeneration on script change\n";
    std::cout << "\n";
    std::cout << "2. add_custom_target() - Custom build targets\n";
    std::cout << "   - Triggers code generation before compilation\n";
    std::cout << "\n";
    std::cout << "3. find_package() - Find external packages\n";
    std::cout << "   - Git integration for version tracking\n";
    std::cout << "\n";
    std::cout << "4. install() - Installation rules\n";
    std::cout << "   cmake --install . --prefix /path/to/install\n";
    std::cout << "\n";
    std::cout << "5. CPack - Packaging support\n";
    std::cout << "   cpack -G ZIP\n";
    std::cout << "\n";

    std::cout << "Build information (from generated build_info.hpp):\n";
    std::cout << "  Version:    " << build_info::version << "\n";
    std::cout << "  Build type: " << build_info::build_type << "\n";
    std::cout << "  Git commit: " << build_info::git_commit << "\n";
    std::cout << "\n";

    std::cout << "Try these commands:\n";
    std::cout << "  cmake --build . --target generate_build_info\n";
    std::cout << "  cmake --install . --prefix ./install\n";
    std::cout << "  cpack -G ZIP\n";
    std::cout << "====================================\n";

    return 0;
}
