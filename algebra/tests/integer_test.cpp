#include "integer.h"

#include <gtest/gtest.h>

#include "algebraic_concepts.h"

using namespace algebra;

TEST(IntegerTest, Basics) {
    EXPECT_TRUE(AbelianRing<Integer>);

    EXPECT_EQ(static_cast<int>(Integer::zero()), 0);
    EXPECT_EQ(static_cast<int>(Integer::one()), 1);
    EXPECT_EQ(static_cast<int>(Integer(3)), 3);
    EXPECT_EQ(static_cast<int>(Integer(-5)), -5);

    int i1 = 3;
    int i2 = -5;
    int i3 = 12;
    Integer I1 = 3;
    Integer I2 = -5;
    Integer I3 = 12;

    EXPECT_EQ(static_cast<int>(I1 + I2), i1 + i2);
    EXPECT_EQ(static_cast<int>(I2 - I3), i2 - i3);
    EXPECT_EQ(static_cast<int>(I1 * I3), i1 * i3);

    auto [p, r] = divide(i3, i2);
    auto [P, R] = divide(I3, I2);

    EXPECT_EQ(static_cast<int>(P), p);
    EXPECT_EQ(static_cast<int>(R), r);
}

TEST(IntegerTest, Printing) {
    std::stringstream sstream;
    std::stringstream Sstream;

    sstream << 125;
    Sstream << Integer(125);

    EXPECT_EQ(Sstream.str(), sstream.str());

    EXPECT_EQ(std::format("{}", Integer(-3)), std::format("{}", -3));
    EXPECT_EQ(std::format("{:3}", Integer(-3)), std::format("{:3}", -3));
}
