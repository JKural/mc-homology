#include "number_theory.h"

#include <gtest/gtest.h>

#include <functional>

using namespace algebra;

TEST(NumberTheoryTest, Primes) {
    auto not_prime = std::not_fn(is_prime);

    EXPECT_PRED1(not_prime, 10);
    EXPECT_PRED1(is_prime, 11);
}

TEST(NumberTheoryTest, Modulo) {
    EXPECT_EQ(mod(17, 7), std::optional {3});
    EXPECT_EQ(mod(-17, 7), std::optional {4});
    EXPECT_EQ(mod(-17, -7), std::nullopt);
    EXPECT_EQ(mod(17, -7), std::nullopt);
}

TEST(NumberTheoryTest, ExtendedGCD) {
    auto result = extended_gcd(12, 18);
    EXPECT_EQ(result.g, 6);
    EXPECT_EQ(result.x, -1);
    EXPECT_EQ(result.y, 1);
    result = extended_gcd(72, 77);
    EXPECT_EQ(result.g, 1);
    EXPECT_EQ(result.x, -31);
    EXPECT_EQ(result.y, 29);
}

TEST(NumberTheoryTest, InverseModN) {
    EXPECT_EQ(inverse_mod(5, 14), std::optional {3});
    EXPECT_EQ(inverse_mod(4, 12), std::nullopt);
}
