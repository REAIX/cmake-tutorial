// 工具模块头文件
// 提供字符串处理工具函数

#pragma once

#include <string>
#include <vector>

// 工具模块命名空间
namespace utils_module {

// 将字符串转换为大写
std::string to_upper(const std::string& str);

// 将字符串转换为小写
std::string to_lower(const std::string& str);

// 按分隔符分割字符串
std::vector<std::string> split(const std::string& str, char delimiter);

}
