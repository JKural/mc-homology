#pragma once

#include <concepts>

namespace algebra {

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
concept AdditiveGroup =
    std::regular<T> && std::default_initializable<T> && requires(T x, T y) {
        { x + y } -> std::convertible_to<T>;
        { x - y } -> std::convertible_to<T>;
        { x += y } -> std::same_as<T&>;
        { x -= y } -> std::same_as<T&>;
        { +x } -> std::convertible_to<T>;
        { -x } -> std::convertible_to<T>;
        { T::zero() } -> std::convertible_to<T>;
    };

/// \brief An abelian ring concept
///
/// `AbelianRing` concept declares that a type models a mathematical
/// definition of an abelian ring, where addition is denoted by `+`
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
/// 5. `x * y == y * x`
template<class T>
concept AbelianRing = AdditiveGroup<T> && requires(T x, T y) {
    { x * y } -> std::convertible_to<T>;
    { x *= y } -> std::same_as<T&>;
    { T::one() } -> std::convertible_to<T>;
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
/// Type `T` satisfies all semantic requirements for 'AbelianRing'
/// and additionally, for `x != T::zero()`
/// 1. `x * (T::one()/x) == T::one()`
template<class T>
concept Field = AbelianRing<T> && requires(T x, T y) {
    { x / y } -> std::convertible_to<T>;
    { x /= y } -> std::same_as<T&>;
};

} // namespace algebra
