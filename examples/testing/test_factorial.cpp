#include "math_lib.h"
#include <cassert>

int main() {
    assert(math::factorial(0) == 1);
    assert(math::factorial(1) == 1);
    assert(math::factorial(5) == 120);
    assert(math::factorial(10) == 3628800);
    return 0;
}
