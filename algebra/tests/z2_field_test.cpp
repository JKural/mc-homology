#include <gtest/gtest.h>

#include "algebraic_concepts.h"
#include "z2_field.h"

using namespace algebra;

TEST(Z2, GroupOperations) {
    Z2 x = 1;
    Z2 y = 0;
    Z2 z = 1;
    Z2 zero = Z2::zero();

    EXPECT_TRUE(AdditiveGroup<Z2>);
    EXPECT_EQ((x + y) + z, x + (y + z));
    EXPECT_EQ(x + zero, x);
    EXPECT_EQ(x + (-x), zero);
    EXPECT_EQ(x + y, y + x);
}

TEST(Z2, RingOperations) {
    Z2 x = 1;
    Z2 y = 0;
    Z2 z = 1;
    Z2 one = Z2::one();

    EXPECT_TRUE(AbelianRing<Z2>);
    EXPECT_EQ((x * y) * z, x * (y * z));
    EXPECT_EQ(x * one, x);
    EXPECT_EQ(x * (y + z), x * y + x * z);
    EXPECT_EQ(x * y, y * x);
}

TEST(Z2, FieldOperations) {
    Z2 x = 1;
    Z2 one = Z2::one();

    EXPECT_TRUE(Field<Z2>);
    EXPECT_EQ(x * one / x, one);
}
