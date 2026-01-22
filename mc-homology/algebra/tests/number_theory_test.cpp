#include "algebra/number_theory.h"

#include <gtest/gtest.h>

#include <functional>
#include <stdexcept>

using namespace algebra;

TEST(NumberTheoryTest, Primes) {
    auto not_prime = std::not_fn(is_prime);

    EXPECT_PRED1(not_prime, 10);
    EXPECT_PRED1(is_prime, 11);
}

TEST(NumberTheoryTest, Division) {
    auto [q1, r1] = divide(17, 7);
    auto [q2, r2] = divide(-17, 7);
    auto [q3, r3] = divide(-17, -7);
    auto [q4, r4] = divide(17, -7);
    EXPECT_EQ(q1, 2);
    EXPECT_EQ(r1, 3);
    EXPECT_EQ(q2, -3);
    EXPECT_EQ(r2, 4);
    EXPECT_EQ(q3, 3);
    EXPECT_EQ(r3, 4);
    EXPECT_EQ(q4, -2);
    EXPECT_EQ(r4, 3);
}

TEST(NumberTheoryTest, Modulo) {
    EXPECT_EQ(modulo(17, 7), 3);
    EXPECT_EQ(modulo(-17, 7), 4);
    EXPECT_EQ(modulo(-17, -7), 4);
    EXPECT_EQ(modulo(17, -7), 3);
    EXPECT_THROW(modulo(17, 0), std::domain_error);
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
