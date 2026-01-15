#pragma once

#include <cmath>
#include <optional>
#include <stdexcept>

namespace algebra {

/// \brief Primality test
///
/// Tests, if a given number is prime, that is divisible only by 1
/// and by itself
///
/// \param n Number to test
///
/// \return `true`, if the number is prime, otherwise `false`
constexpr bool is_prime(int n) noexcept {
    if (n < 2) {
        return false;
    }
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

/// \brief Result struct for the division operation
template<class T>
struct DivResult {
    T quotient = {};
    T remainder = {};
};

/// \brief Calculates quotient and remainder of two numbers
///
/// A function calculating numbers `quotient` and `remainder` satisfying
/// `a == quotient * n + remainder` and 0 <= remainder < abs(n)
///
/// \param a An integer
/// \param n An integer
///
/// \return A struct holding three numbers:
///     `quotient`: the quotient
///     `remainder`: the remainder
constexpr DivResult<int> divide(int a, int b) {
    if (b != 0) [[likely]] {
        auto [q, r] = std::div(a, b);
        if (r < 0) {
            q -= b > 0 ? 1 : -1;
            r += b > 0 ? b : -b;
        }
        return {.quotient = q, .remainder = r};
    } else [[unlikely]] {
        throw std::domain_error("Division by 0");
    }
}

/// \brief Calculates the remainder of a by n
///
/// \param a An integer
/// \param n an integer
///
/// \return The remainder
constexpr int modulo(int a, int n) {
    return divide(a, n).remainder;
}

/// \brief Result struct for the extended gcd algorithm
struct ExtendedGCDResult {
    int g = 0;
    int x = 0;
    int y = 0;
};

/// \brief Extended Euclidean algorithm
///
/// Calculates the greatest common divisor `g` and integers `x`, `y`
/// such that `a*x + b*y == g`.
///
/// \param a An integer
/// \param b An integer
///
/// \return A struct holding three numbers:
///     `g`: the gcd
///     `x`: first coefficient
///     `y`: second coefficient
constexpr ExtendedGCDResult extended_gcd(int a, int b) noexcept {
    a = std::abs(a);
    b = std::abs(b);
    int x1 = 1;
    int x2 = 0;
    int y1 = 0;
    int y2 = 1;
    while (b > 0) {
        auto [q, r] = divide(a, b);
        std::tie(a, b) = std::pair {b, r};
        std::tie(x1, x2) = std::pair {x2, x1 - q * x2};
        std::tie(y1, y2) = std::pair {y2, y1 - q * y2};
    }
    return {.g = a, .x = x1, .y = y1};
}

/// \brief Inverse modulo n
///
/// Calculates inverse of `a` mod `n`.
///
/// \param a Inverted number
/// \param n The modulus
///
/// \return If `a` is invertible mod `n`, returns the inverse, `nullopt` otherwize
constexpr std::optional<int> inverse_mod(int a, int n) noexcept {
    auto [g, x, _] = extended_gcd(a, n);
    if (g == 1) [[likely]] {
        return x;
    } else [[unlikely]] {
        return std::nullopt;
    }
}

} // namespace algebra
