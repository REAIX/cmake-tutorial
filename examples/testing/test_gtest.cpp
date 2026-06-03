#include <gtest/gtest.h>
#include "math_lib.h"

// 测试 math::add 函数
TEST(MathTest, Add) {
    EXPECT_EQ(math::add(2, 3), 5);
    EXPECT_EQ(math::add(-1, 1), 0);
    EXPECT_EQ(math::add(0, 0), 0);
    EXPECT_EQ(math::add(100, 200), 300);
    EXPECT_EQ(math::add(-5, -7), -12);
}

// 测试 math::multiply 函数
TEST(MathTest, Multiply) {
    EXPECT_EQ(math::multiply(2, 3), 6);
    EXPECT_EQ(math::multiply(-1, 5), -5);
    EXPECT_EQ(math::multiply(0, 100), 0);
    EXPECT_EQ(math::multiply(7, 8), 56);
    EXPECT_EQ(math::multiply(-3, -4), 12);
}

// 测试 math::factorial 函数
TEST(MathTest, Factorial) {
    EXPECT_EQ(math::factorial(0), 1);
    EXPECT_EQ(math::factorial(1), 1);
    EXPECT_EQ(math::factorial(5), 120);
    EXPECT_EQ(math::factorial(10), 3628800);
    EXPECT_EQ(math::factorial(3), 6);
}
