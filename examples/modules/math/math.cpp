// 数学模块实现文件

#include "math_lib.h"
#include <cmath>

namespace math_module {

// 计算平方值
double square(double x) {
    return x * x;
}

// 计算幂运算（使用标准库 pow 函数）
double power(double base, int exponent) {
    return std::pow(base, exponent);
}

// 判断是否为质数
// 使用优化的质数检测算法
bool is_prime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    
    // 检查 6k ± 1 形式的因子
    // 使用 long long 转换避免 i * i 在 n 接近 INT_MAX 时溢出
    for (int i = 5; (long long)i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }
    return true;
}

}
