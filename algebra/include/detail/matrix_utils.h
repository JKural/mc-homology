#pragma once

#include "algebraic_concepts.h"
#include "matrix.h"

namespace algebra {
namespace detail {

template<AdditiveGroup T>
constexpr std::optional<std::size_t> first_nonzero_submatrix_column_coefficient(
    Matrix<T> const& matrix,
    std::size_t i,
    std::size_t j
) noexcept {
    for (auto k = i; k < matrix.nrows(); ++k) {
        if (matrix[k, j] != T::zero()) {
            return k;
        }
    }
    return std::nullopt;
}

template<class T>
constexpr void submatrix_swap_rows(
    Matrix<T>& matrix,
    std::size_t i1,
    std::size_t i2,
    std::size_t j
) noexcept {
    for (auto l = j; l < matrix.ncols(); ++l) {
        namespace rs = std::ranges;
        rs::swap(matrix[i1, j], matrix[i2, j]);
    }
}

template<AbelianRing T>
constexpr void submatrix_add_row(
    Matrix<T>& matrix,
    T const& mult,
    std::size_t i1,
    std::size_t i2,
    std::size_t j
) noexcept {
    for (auto l = j; l < matrix.ncols(); ++l) {
        matrix[i2, j] += mult * matrix[i1, j];
    }
}

} // namespace detail
} // namespace algebra
