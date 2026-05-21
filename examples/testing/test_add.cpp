#include "math.h"
#include <cassert>

int main() {
    assert(math::add(2, 3) == 5);
    assert(math::add(-1, 1) == 0);
    assert(math::add(0, 0) == 0);
    assert(math::add(100, 200) == 300);
    return 0;
}
