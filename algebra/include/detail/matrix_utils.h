#pragma once

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

template<EuclideanDomain T>
constexpr std::pair<std::size_t, std::size_t> minimal_nonzero_submatrix_element(
    Matrix<T> const& matrix,
    std::size_t i,
    std::size_t j
) noexcept {
    std::pair min = {i, j};
    for (std::size_t k = i; k < matrix.nrows(); ++k) {
        for (std::size_t l = j; l < matrix.ncols(); ++l) {
            if (matrix[k, l].euclidean_function() != T::zero()
                && (matrix[min.first, min.second].euclidean_function()
                        > matrix[k, l].euclidean_function()
                    || matrix[min.first, min.second] == T::zero())) {
                min = {k, l};
            }
        }
    }
    return min;
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
        rs::swap(matrix[i1, l], matrix[i2, l]);
    }
}

template<class T>
constexpr void submatrix_swap_cols(
    Matrix<T>& matrix,
    std::size_t j1,
    std::size_t j2,
    std::size_t i
) noexcept {
    for (auto k = i; k < matrix.nrows(); ++k) {
        namespace rs = std::ranges;
        rs::swap(matrix[k, j1], matrix[k, j2]);
    }
}

template<CommutativeRing T>
constexpr void submatrix_add_row(
    Matrix<T>& matrix,
    T const& mult,
    std::size_t source_row,
    std::size_t target_row,
    std::size_t j
) noexcept {
    for (auto l = j; l < matrix.ncols(); ++l) {
        matrix[target_row, l] += mult * matrix[source_row, l];
    }
}

template<CommutativeRing T>
constexpr void submatrix_add_col(
    Matrix<T>& matrix,
    T const& mult,
    std::size_t source_col,
    std::size_t target_col,
    std::size_t i
) noexcept {
    for (auto k = i; k < matrix.nrows(); ++k) {
        matrix[k, target_col] += mult * matrix[k, source_col];
    }
}

template<CommutativeRing T>
constexpr void submatrix_multiply_row(
    Matrix<T>& matrix,
    T const& mult,
    std::size_t i,
    std::size_t j
) noexcept {
    for (auto l = j; l < matrix.ncols(); ++l) {
        matrix[i, l] *= mult;
    }
}

template<CommutativeRing T>
constexpr void submatrix_multiply_col(
    Matrix<T>& matrix,
    T const& mult,
    std::size_t j,
    std::size_t i
) noexcept {
    for (auto k = i; k < matrix.ncols(); ++k) {
        matrix[k, j] *= mult;
    }
}

} // namespace detail
} // namespace algebra
