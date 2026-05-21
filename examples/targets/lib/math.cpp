// 数学库实现文件

#include "math.h"

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
long long factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

}
