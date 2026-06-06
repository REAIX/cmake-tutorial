#include "math_lib.h"

namespace math {

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

long long factorial(int n) {
    if (n < 0) return 0;
    if (n > 20) return -1;  // 超出 long long 范围，返回 -1 表示错误
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

}
