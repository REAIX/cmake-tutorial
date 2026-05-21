#include "math.h"

namespace math {

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

long long factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

}
