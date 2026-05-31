// Header-Only 库示例
// 演示 INTERFACE 库和 configure_file 的用法

#pragma once

#include "config.h"

#include <string>
#include <sstream>
#include <iomanip>

namespace format_lib {

inline std::string format_number(int value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

inline std::string format_hex(int value) {
    std::ostringstream oss;
    oss << "0x" << std::hex << std::uppercase << value;
    return oss.str();
}

} // namespace format_lib
