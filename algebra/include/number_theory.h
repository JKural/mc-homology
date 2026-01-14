#pragma once

#include <cmath>
#include <optional>

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

/// \brief Modulo
///
/// A function calculating modulo by a positive number `n`,
/// such that for the result `r` the following holds: `0 <= r < n`.
///
/// \param a An integer
/// \param n A nonnegative integer
///
/// \return An optional holding the result if n is positive,
/// `nullopt` otherwise
constexpr std::optional<int> mod(int a, int n) noexcept {
    if (n >= 1) [[likely]] {
        return (a % n + n) % n;
    } else [[unlikely]] {
        return std::nullopt;
    }
}

struct ExtendedGCDRV {
    int g = 1;
    int x = 1;
    int y = 1;
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
constexpr ExtendedGCDRV extended_gcd(int a, int b) noexcept {
    a = std::abs(a);
    b = std::abs(b);
    int x1 = 1;
    int x2 = 0;
    int y1 = 0;
    int y2 = 1;
    while (b > 0) {
        int q = a / b;
        int c = b;
        int x3 = x2;
        int y3 = y2;
        b = a - q * b;
        x2 = x1 - q * x2;
        y2 = y1 - q * y2;
        a = c;
        x1 = x3;
        y1 = y3;
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
