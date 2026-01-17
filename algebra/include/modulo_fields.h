#pragma once

#include <format>
#include <iostream>
#include <stdexcept>

#include "algebraic_concepts.h"
#include "number_theory.h"

namespace algebra {

/// \brief Field of integers modulo P.
///
/// A class modeling fields of integers modulo P, where P is a prime
/// number.
///
/// \param P A prime number
template<int P>
class ZModP {
    static_assert(is_prime(P));

public:
    /// \brief Returns 0.
    constexpr ZModP() = default;

    /// \brief Returns n mod P.
    constexpr ZModP(int n) noexcept : m_inner_representation(modulo(n, P)) {}

    /// \brief Returns the modulus P.
    constexpr static int p() noexcept {
        return P;
    }

    /// \brief Returns 0.
    constexpr static ZModP zero() noexcept {
        return ZModP(0);
    }

    /// \brief Returns 1.
    constexpr static ZModP one() noexcept {
        return ZModP(1);
    }

    /// \brief Returns the underlying integer.
    constexpr explicit operator int() const noexcept {
        return m_inner_representation;
    }

    constexpr bool operator==(ZModP const&) const = default;

    constexpr ZModP& operator+=(ZModP lhs) noexcept {
        m_inner_representation =
            modulo(m_inner_representation + lhs.m_inner_representation, P);
        return *this;
    }

    constexpr ZModP& operator-=(ZModP lhs) noexcept {
        m_inner_representation =
            modulo(m_inner_representation - lhs.m_inner_representation, P);
        return *this;
    }

    constexpr ZModP operator+() const noexcept {
        return *this;
    }

    constexpr ZModP operator-() const noexcept {
        return ZModP(modulo(-m_inner_representation, P));
    }

    constexpr ZModP& operator*=(ZModP lhs) noexcept {
        m_inner_representation =
            modulo(m_inner_representation * lhs.m_inner_representation, P);
        return *this;
    }

    constexpr int euclidean_function() const noexcept {
        return 1;
    }

    constexpr ZModP& operator/=(ZModP lhs) {
        auto inverse = inverse_mod(lhs.m_inner_representation, P);
        if (!inverse) [[unlikely]] {
            throw std::domain_error("Division by 0");
        }
        return *this *= *inverse;
    }

private:
    int m_inner_representation = 0;
};

template<int P>
constexpr ZModP<P> operator+(ZModP<P> lhs, ZModP<P> rhs) noexcept {
    return lhs += rhs;
}

template<int P>
constexpr ZModP<P> operator-(ZModP<P> lhs, ZModP<P> rhs) noexcept {
    return lhs -= rhs;
}

template<int P>
constexpr ZModP<P> operator*(ZModP<P> lhs, ZModP<P> rhs) noexcept {
    return lhs *= rhs;
}

template<int P>
constexpr ZModP<P> operator/(ZModP<P> lhs, ZModP<P> rhs) {
    return lhs /= rhs;
}

template<int P>
std::ostream& operator<<(std::ostream& output, ZModP<P> x) {
    return output << static_cast<int>(x);
}

template<int P>
constexpr inline bool is_commutative_v<ZModP<P>> = true;

} // namespace algebra

/// \brief Formatter for ZModP type
///
/// Allows use of `std::format` with the `ZModP` type. The format
/// syntax is the same, as in the case of `int`.
template<int P>
struct std::formatter<algebra::ZModP<P>>: public std::formatter<int> {
    template<class FmtContext>
    FmtContext::iterator format(algebra::ZModP<P> x, FmtContext& ctx) const {
        return std::formatter<int>::format(static_cast<int>(x), ctx);
    }
};
