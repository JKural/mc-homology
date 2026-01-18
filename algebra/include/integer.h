/// \file integer.h
/// \brief A file containing Integer implementation satisfying
///        algebraic concepts

#pragma once

#include <compare>
#include <cstdlib>
#include <format>
#include <iostream>

#include "algebraic_concepts.h"

namespace algebra {

/// \brief Class of Integers
///
/// A class implementing integers, while additionally satisfying
/// EuclideanDomain constraint
class Integer {
public:
    /// \brief Returns 0
    constexpr Integer() = default;

    /// \brief Returns k
    constexpr Integer(int k) noexcept : m_inner_representation(k) {}

    /// \brief Returns 0
    constexpr static Integer zero() noexcept {
        return Integer(0);
    }

    /// \brief Returns 1
    constexpr static Integer one() noexcept {
        return Integer(1);
    }

    /// \brief Returns the underlying integer
    constexpr explicit operator int() noexcept {
        return m_inner_representation;
    }

    /// \brief Compares two integers for equality
    constexpr bool operator==(Integer const&) const = default;

    /// \brief Compares two integers and returns their ordering
    constexpr std::strong_ordering operator<=>(Integer const&) const = default;

    /// \brief Adds rhs to itself
    constexpr Integer& operator+=(Integer rhs) noexcept {
        m_inner_representation += rhs.m_inner_representation;
        return *this;
    }

    /// \brief Subtracts rhs from itself
    constexpr Integer& operator-=(Integer rhs) noexcept {
        m_inner_representation -= rhs.m_inner_representation;
        return *this;
    }

    /// \brief Returns a copy of self
    constexpr Integer operator+() const noexcept {
        return *this;
    }

    /// \brief Returns a negation of self
    constexpr Integer operator-() const noexcept {
        return Integer(-m_inner_representation);
    }

    /// \brief Multiplies itself by rhs
    constexpr Integer& operator*=(Integer rhs) noexcept {
        m_inner_representation *= rhs.m_inner_representation;
        return *this;
    }

    /// \brief Euclidean function for integers
    ///
    /// Euclidean function for integers, in this case the absolute
    /// value.
    constexpr int euclidean_function() const noexcept {
        using std::abs;
        return abs(m_inner_representation);
    }

private:
    int m_inner_representation = 0;
};

/// \brief Adds two integers
constexpr Integer operator+(Integer lhs, Integer rhs) noexcept {
    return lhs += rhs;
}

/// \brief Subtracts two integers
constexpr Integer operator-(Integer lhs, Integer rhs) noexcept {
    return lhs -= rhs;
}

/// \brief Multiplies two integers
constexpr Integer operator*(Integer lhs, Integer rhs) noexcept {
    return lhs *= rhs;
}

/// \brief Returns absolute value
constexpr Integer abs(Integer k) noexcept {
    return std::abs(static_cast<int>(k));
}

/// \brief Returns the result of integer division for Integers a and b
///
/// Returns unique `q` and `r` satisfying
/// 1. `a == q * b + r`
/// 2. `0 <= r < b.euclidean_function()`
constexpr DivResult<Integer> divide(Integer a, Integer b) {
    auto div_result = divide(static_cast<int>(a), static_cast<int>(b));
    return DivResult<Integer> {
        .quotient = div_result.quotient,
        .remainder = div_result.remainder
    };
}

/// \brief Returns the remainder of division of a by n
///
/// Returns an integer r, 0 <= r < n satisfying
/// `a == q * n + r`
constexpr Integer modulo(Integer a, Integer n) {
    return divide(a, n).remainder;
}

/// \brief A marker that multiplying the integers is commutative
template<>
constexpr inline bool is_commutative_v<Integer> = true;

/// \brief Outputs an integer to a stream
inline std::ostream& operator<<(std::ostream& output, Integer k) {
    return output << static_cast<int>(k);
}

} // namespace algebra

/// \brief Formatter for Integer type
///
/// Allows use of `std::format` with the `Integer` type. The format
/// syntax is the same, as in the case of `int`.
template<>
struct std::formatter<algebra::Integer>: public std::formatter<int> {
    /// \brief Formats an integer
    ///
    /// Formats an integer using a formatting syntax for int.
    template<class FmtContext>
    FmtContext::iterator format(algebra::Integer k, FmtContext& ctx) const {
        return std::formatter<int>::format(static_cast<int>(k), ctx);
    }
};
