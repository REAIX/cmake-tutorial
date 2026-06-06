// 数学库实现文件

#include "math_lib.h"

namespace math {

// 加法运算
int add(int a, int b) {
    return a + b;
}

// 乘法运算
int multiply(int a, int b) {
    return a * b;
}

// 阶乘运算（递归实现）
// 注意：n > 20 时 long long 会溢出，此处做上界保护
long long factorial(int n) {
    if (n < 0) return 0;
    if (n > 20) return -1;  // 超出 long long 范围，返回 -1 表示错误
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

}
