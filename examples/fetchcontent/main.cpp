#include <iostream>
#include <fmt/core.h>

int main() {
    std::cout << "====================================\n";
    std::cout << "  08 - FetchContent Dependency\n";
    std::cout << "====================================\n";
    std::cout << "\n";

    std::cout << "Key concepts demonstrated:\n";
    std::cout << "1. include(FetchContent) - Load FetchContent module\n";
    std::cout << "2. FetchContent_Declare() - Define external dependency\n";
    std::cout << "   - GIT_REPOSITORY: URL of the Git repo\n";
    std::cout << "   - GIT_TAG: specific tag/branch/commit\n";
    std::cout << "3. FetchContent_MakeAvailable() - Fetch and add to build\n";
    std::cout << "4. target_link_libraries() - Link fetched library\n";
    std::cout << "\n";

    fmt::print("Using fmt library (fetched via FetchContent):\n");
    fmt::print("  Hello from {}!\n", "fmt");
    fmt::print("  Pi is approximately {:.2f}\n", 3.14159);
    fmt::print("  {} + {} = {}\n", 1, 2, 3);
    std::cout << "\n";

    std::cout << "FetchContent vs find_package():\n";
    std::cout << "  FetchContent: downloads and builds from source\n";
    std::cout << "  find_package: locates pre-installed libraries\n";
    std::cout << "\n";

    std::cout << "FetchContent best practices:\n";
    std::cout << "  - Pin specific GIT_TAG for reproducibility\n";
    std::cout << "  - Use FetchContent_MakeAvailable (CMake 3.14+)\n";
    std::cout << "  - Prefer over ExternalProject for deps\n";
    std::cout << "====================================\n";

    return 0;
}
