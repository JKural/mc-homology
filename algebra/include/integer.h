#pragma once

#include <compare>
#include <cstdlib>
#include <format>
#include <iostream>

#include "algebraic_concepts.h"
#include "number_theory.h"

namespace algebra {

class Integer {
public:
    constexpr Integer() = default;

    constexpr Integer(int k) noexcept : m_inner_representation(k) {}

    constexpr static Integer zero() noexcept {
        return Integer(0);
    }

    constexpr static Integer one() noexcept {
        return Integer(1);
    }

    constexpr explicit operator int() noexcept {
        return m_inner_representation;
    }

    constexpr bool operator==(Integer const&) const = default;

    constexpr std::strong_ordering operator<=>(Integer const&) const = default;

    constexpr Integer& operator+=(Integer rhs) noexcept {
        m_inner_representation += rhs.m_inner_representation;
        return *this;
    }

    constexpr Integer& operator-=(Integer rhs) noexcept {
        m_inner_representation -= rhs.m_inner_representation;
        return *this;
    }

    constexpr Integer operator+() const noexcept {
        return *this;
    }

    constexpr Integer operator-() const noexcept {
        return Integer(-m_inner_representation);
    }

    constexpr Integer& operator*=(Integer rhs) noexcept {
        m_inner_representation *= rhs.m_inner_representation;
        return *this;
    }

    constexpr int euclidean_function() const noexcept {
        using std::abs;
        return abs(m_inner_representation);
    }

private:
    int m_inner_representation = 0;
};

constexpr Integer operator+(Integer lhs, Integer rhs) noexcept {
    return lhs += rhs;
}

constexpr Integer operator-(Integer lhs, Integer rhs) noexcept {
    return lhs -= rhs;
}

constexpr Integer operator*(Integer lhs, Integer rhs) noexcept {
    return lhs *= rhs;
}

constexpr Integer abs(Integer k) noexcept {
    return std::abs(static_cast<int>(k));
}

constexpr DivResult<Integer> divide(Integer a, Integer b) {
    auto div_result = divide(static_cast<int>(a), static_cast<int>(b));
    return DivResult<Integer> {
        .quotient = div_result.quotient,
        .remainder = div_result.remainder
    };
}

constexpr Integer modulo(Integer a, Integer n) {
    return divide(a, n).remainder;
}

template<>
constexpr inline bool is_commutative_v<Integer> = true;

std::ostream& operator<<(std::ostream& output, Integer k);

} // namespace algebra

template<>
struct std::formatter<algebra::Integer>: public std::formatter<int> {
    template<class FmtContext>
    FmtContext::iterator format(algebra::Integer k, FmtContext& ctx) const {
        return std::formatter<int>::format(static_cast<int>(k), ctx);
    }
};
