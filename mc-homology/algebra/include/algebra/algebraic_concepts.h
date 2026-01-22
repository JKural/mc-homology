/// \file algebraic_concepts.h
/// \brief A file defining multiple concepts for algebraic structures

#pragma once

#include <concepts>

#include "algebra/number_theory.h"

namespace algebra {

/// \brief Helper variable template that can be used as a mark that
///        class' main operation (addition or multiplication) is abelian
template<class T>
constexpr inline bool is_commutative_v = false;

/// \brief An abelian structure concept
///
/// `Abelian` concept declares that a type's main operation is abelian,
/// that is, for any `T x`, `T y` and operation `@` (`+` lub `-`) we
/// have `x @ y == y @ x`.
///
/// # Semantic requirements
///
/// For `x` and `y` and operation `@`
/// 1. `x @ y == y @ x`
template<class T>
concept Commutative = is_commutative_v<T>;

/// \brief A group concept
///
/// `Group` concept declares that a type models a mathematical
/// definition of a group, that is an abelian group that,
/// in which the group operation is expressed with '*'.
///
/// # Semantic requirements
///
/// For `x`, `y`, `z` of type `T`
/// 1. `(x * y) * z == x * (y * z)`
/// 2. `x * T::one() == T::one() * x == x`
/// 3. `x * (T::one() / x) == (T::one() / x) * x == T::one()`
template<class T>
concept Group =
    std::regular<T> && std::default_initializable<T> && requires(T x, T y) {
        { x * y } -> std::convertible_to<T>;
        { x / y } -> std::convertible_to<T>;
        { x *= y } -> std::same_as<T&>;
        { x /= y } -> std::same_as<T&>;
        { T::one() } -> std::convertible_to<T>;
    };

/// \brief An additive group concept
///
/// `AdditiveGroup` concept declares that a type models a mathematical
/// definition of an additive group, that is an abelian group that,
/// in which the group operation is expressed with '+'.
///
/// # Semantic requirements
///
/// For `x`, `y`, `z` of type `T`
/// 1. `(x + y) + z == x + (y + z)`
/// 2. `x + T::zero() == T::zero() + x == x`
/// 3. `x + (-x) == (-x) + x == T::zero()`
/// 4. `x + y == y + x`
template<class T>
concept AdditiveGroup = std::regular<T> && std::default_initializable<T>
    && Commutative<T> && requires(T x, T y) {
           { x + y } -> std::convertible_to<T>;
           { x - y } -> std::convertible_to<T>;
           { x += y } -> std::same_as<T&>;
           { x -= y } -> std::same_as<T&>;
           { +x } -> std::convertible_to<T>;
           { -x } -> std::convertible_to<T>;
           { T::zero() } -> std::convertible_to<T>;
       };

/// \brief A ring concept
///
/// `Ring` concept declares that a type models a mathematical
/// definition of a ring, where addition is denoted by `+`
/// and multiplication by `*`.
///
/// # Sematic requirements
///
/// Type `T` satisfies all semantic requirements for `AdditiveGroup`
/// and additionally, for `x`, `y`, `z` of type `T`
/// 1. `(x * y) * z == x * (y * z)`
/// 2. `x * (y + z) == x * y + x * z`
/// 3. `(x + y) * z == x * z + y * z`
/// 4. `T::one() * x == x * T::one() == x`
template<class T>
concept Ring = AdditiveGroup<T> && requires(T x, T y) {
    { x * y } -> std::convertible_to<T>;
    { x *= y } -> std::same_as<T&>;
    { T::one() } -> std::convertible_to<T>;
};

/// \brief An commutative ring concept
///
/// `CommutativeRing` concept declares that a type models a mathematical
/// definition of a commutative ring, where addition is denoted by `+`
/// and multiplication by `*`.
///
/// # Sematic requirements
///
/// Type `T` satisfies all semantic requirements for `Ring`
/// and additionally, for `x`, `y` of type `T`
/// 1. `x * y == y * x`
template<class T>
concept CommutativeRing = Ring<T> && Commutative<T>;

/// \brief An euclidean domain concept
///
/// `EuclideanDomain` concept declares that a type models a
/// mathematical definition of an Euclidean domain, that is a
/// commutative ring, where we can define an euclidean function f, that
/// satisfies two conditions:
/// 1. For a and nonzero b there exist q and r satisfying
///    a = q * b + r and f(r) < f(b)
/// 2. For a and b nonzero we have f(a) <= f(b)
///
/// Additionally, we require function `divide`, which returns the
/// numbers q and r.
///
/// # Semantic requirements
///
/// Type 'T' satisfies all semantic requirements for `CommutativeRing`
/// and additionally, for `x` and `y != T::zero()`
/// 1. `x = divide(x, y).quotient * y + divide(x, y).remainder`
/// 2. `x.euclidean_function() <= (x * y).euclidean_function()`
template<class T>
concept EuclideanDomain = CommutativeRing<T> && requires(T a, T b, T x, T y) {
    { divide(x, y) } -> std::same_as<DivResult<T>>;
    { x.euclidean_function() } -> std::same_as<int>;
};

/// \brief A field concept
///
/// `Field` concept declares that a type models a methematical
/// definition of a field, where addition is denoted by `+` and
/// multiplication is denoted by `*`. Additionaly, the is a divison
/// operator '/'
///
/// # Semantic requirements
///
/// Type `T` satisfies all semantic requirements for `EuclideanDomain`
/// and additionally, for `x != T::zero()`
/// 1. `x * (T::one()/x) == T::one()`
template<class T>
concept Field = CommutativeRing<T> && requires(T x, T y) {
    { x / y } -> std::convertible_to<T>;
    { x /= y } -> std::same_as<T&>;
};

} // namespace algebra
