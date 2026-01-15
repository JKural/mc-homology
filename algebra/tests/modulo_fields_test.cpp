#include "modulo_fields.h"

#include <gtest/gtest.h>

#include "algebraic_concepts.h"

using namespace algebra;
using Z7 = ZModP<7>;

TEST(ZModPTest, GroupOperations) {
    Z7 x = 3;
    Z7 y = 5;
    Z7 z = 2;
    Z7 zero = Z7::zero();

    EXPECT_TRUE(AdditiveGroup<Z7>);
    EXPECT_EQ((x + y) + z, x + (y + z));
    EXPECT_EQ(x + zero, x);
    EXPECT_EQ(x + (-x), zero);
    EXPECT_EQ(x + y, y + x);
}

TEST(ZModPTest, RingOperations) {
    Z7 x = 3;
    Z7 y = 5;
    Z7 z = 2;
    Z7 one = Z7::one();

    EXPECT_TRUE(CommutativeRing<Z7>);
    EXPECT_EQ((x * y) * z, x * (y * z));
    EXPECT_EQ(x * one, x);
    EXPECT_EQ(x * (y + z), x * y + x * z);
    EXPECT_EQ(x * y, y * x);
}

TEST(ZModPTest, FieldOperations) {
    Z7 x = 3;
    Z7 one = Z7::one();

    EXPECT_TRUE(Field<Z7>);
    EXPECT_EQ(x * one / x, one);
}
