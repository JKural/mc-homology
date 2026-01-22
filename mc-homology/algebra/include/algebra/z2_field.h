/// \file z2_field.h
/// \brief A file containing optimized implementation of Z2 field

#pragma once

#include "algebra/modulo_fields.h"

namespace algebra {

/// \brief Template specialization for P == 2
template<>
class ZModP<2> {
public:
    /// \brief Returns 0.
    constexpr ZModP() = default;

    /// \brief Retur>ns n mod P.
    constexpr ZModP(int n) noexcept : m_inner_representation(modulo(n, 2)) {}

    /// \brief Returns the modulus P.
    constexpr static int p() noexcept {
        return 2;
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

    /// \brief Equality comparison
    constexpr bool operator==(ZModP const&) const = default;

    /// \brief Adds rhs to itself
    constexpr ZModP& operator+=(ZModP rhs) noexcept {
        m_inner_representation ^= rhs.m_inner_representation;
        return *this;
    }

    /// \brief Subtracts rhs from itself
    constexpr ZModP& operator-=(ZModP rhs) noexcept {
        m_inner_representation ^= rhs.m_inner_representation;
        return *this;
    }

    /// \brief Returns a copy of itself
    constexpr ZModP operator+() const noexcept {
        return *this;
    }

    /// \brief Returns a negation of itself
    constexpr ZModP operator-() const noexcept {
        return *this;
    }

    /// \brief Multiplies itself by rhs
    constexpr ZModP& operator*=(ZModP rhs) noexcept {
        m_inner_representation &= rhs.m_inner_representation;
        return *this;
    }

    /// \brief Euclidean function for fields
    ///
    /// Euclidean function for fields is constantly equal to 1.
    constexpr int euclidean_function() const noexcept {
        return 1;
    }

    /// \brief Divides itself by rhs
    constexpr ZModP& operator/=(ZModP rhs) {
        if (!rhs.m_inner_representation) [[unlikely]] {
            throw std::domain_error("Division by 0");
        }
        return *this;
    }

private:
    bool m_inner_representation = false;
};

/// \brief Alias for ZModP<2>
using Z2 = ZModP<2>;

} // namespace algebra
