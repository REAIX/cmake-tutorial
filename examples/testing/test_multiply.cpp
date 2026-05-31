#include "math_lib.h"
#include <cassert>

int main() {
    assert(math::multiply(2, 3) == 6);
    assert(math::multiply(-1, 5) == -5);
    assert(math::multiply(0, 100) == 0);
    assert(math::multiply(7, 8) == 56);
    return 0;
}
