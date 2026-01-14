#pragma once

#include "modulo_fields.h"

namespace algebra {

template<>
class ZModP<2> {
public:
    /// \brief Returns 0.
    constexpr ZModP() = default;

    /// \brief Retur>ns n mod P.
    constexpr ZModP(int n) noexcept :
        m_inner_representation(mod(n, 2).value()) {}

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

    constexpr bool operator==(ZModP const&) const = default;

    constexpr ZModP& operator+=(ZModP lhs) noexcept {
        m_inner_representation ^= lhs.m_inner_representation;
        return *this;
    }

    constexpr ZModP& operator-=(ZModP lhs) noexcept {
        m_inner_representation ^= lhs.m_inner_representation;
        return *this;
    }

    constexpr ZModP operator+() const noexcept {
        return *this;
    }

    constexpr ZModP operator-() const noexcept {
        return *this;
    }

    constexpr ZModP& operator*=(ZModP lhs) noexcept {
        m_inner_representation &= lhs.m_inner_representation;
        return *this;
    }

    constexpr ZModP& operator/=(ZModP lhs) {
        if (!lhs.m_inner_representation) [[unlikely]] {
            throw std::domain_error("Division by 0");
        }
        return *this;
    }

private:
    bool m_inner_representation = false;
};

using Z2 = ZModP<2>;

} // namespace algebra
