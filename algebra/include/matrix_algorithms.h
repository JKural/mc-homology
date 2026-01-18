/// \file matrix_algorithms.h
/// \brief A file containing selected matrix algorithms

#pragma once

#include <concepts>
#include <utility>

#include "algebraic_concepts.h"
#include "detail/matrix_utils.h"
#include "matrix.h"

namespace algebra {

/// \brief Transforms a matrix into a row echolon form in place
///
/// Transforms a matrix into a row echelon form in place and returns
/// the number of non-zero rows.
///
/// \param[inout] matrix Matrix to be transformed
///
/// \return The number of non-zero rows
template<Field T>
constexpr std::size_t row_echelon_form(std::in_place_t, Matrix<T>& matrix) {
    std::size_t i = 0;
    for (std::size_t j = 0; j < matrix.ncols(); ++j) {
        auto maybe_i =
            detail::first_nonzero_submatrix_column_coefficient(matrix, i, j);
        if (!maybe_i) {
            continue;
        }
        if (*maybe_i != i) {
            detail::submatrix_swap_rows(matrix, i, *maybe_i, j);
        }
        for (auto k = i + 1; k < matrix.nrows(); ++k) {
            auto mult = -matrix[k, j] / matrix[i, j];
            detail::submatrix_add_row(matrix, mult, i, k, j);
        }
        ++i;
    }
    return i;
}

/// \brief Result struct for the row echelon algorithm
template<class T>
struct RowEchelonFormResult {
    /// \brief Row echelon form of a matrix
    Matrix<T> row_echelon_form = {};

    /// \brief Number of non-empty rows of the matrix in row echelon
    ///        form
    std::size_t non_empty_rows = 0;
};

/// \brief Transforms a matrix into a row echelon form
///
/// Transforms a matrix into a row echelon form and returns
/// the number of non-zero rows.
///
/// \param matrix Matrix to be transformed
///
/// \return A struct containing two fields
/// 1. row_echelon_form The transformed matrix
/// 2. non_empty_rows the number of non-zero rows
template<Field T>
constexpr RowEchelonFormResult<T> row_echelon_form(Matrix<T> matrix) {
    auto i = row_echelon_form(std::in_place, matrix);
    return RowEchelonFormResult {
        .row_echelon_form = matrix,
        .non_empty_rows = i
    };
}

/// \brief Transforms a matrix into a Smith form in place
///
/// Transforms a matrix into a Smith form in place and returns
/// the smaller of non-zero rows or columns
///
/// \param[inout] matrix Matrix to be transformed
///
/// \return The number of non-zero rows or columns
template<EuclideanDomain T>
constexpr std::size_t smith_form(std::in_place_t, Matrix<T>& matrix) {
    std::size_t k = 0;
    auto move_min_to_corner = [&] {
        auto min_element =
            detail::minimal_nonzero_submatrix_element(matrix, k, k);
        if (matrix[min_element.first, min_element.second] == T::zero()) {
            return;
        }
        if (min_element.first != k) {
            detail::submatrix_swap_rows(matrix, k, min_element.first, k);
        }
        if (min_element.second != k) {
            detail::submatrix_swap_cols(matrix, k, min_element.second, k);
        }
    };
    for (; k < std::min(matrix.nrows(), matrix.ncols()); ++k) {
        bool col_all_zeros = false;
        while (!col_all_zeros) {
            move_min_to_corner();
            if (matrix[k, k] == T::zero()) {
                return k;
            }
            col_all_zeros = true;
            for (std::size_t i = k + 1; i < matrix.nrows(); ++i) {
                auto [q, r] = divide(matrix[i, k], matrix[k, k]);
                detail::submatrix_add_row(matrix, -q, k, i, k);
                if (r != T::zero()) {
                    col_all_zeros = false;
                }
            }
        }
        bool row_all_zeros = false;
        while (!row_all_zeros) {
            move_min_to_corner();
            if (matrix[k, k] == T::zero()) {
                return k;
            }
            row_all_zeros = true;
            for (std::size_t j = k + 1; j < matrix.ncols(); ++j) {
                auto [q, r] = divide(matrix[k, j], matrix[k, k]);
                detail::submatrix_add_col(matrix, -q, k, j, k);
                if (r != T::zero()) {
                    row_all_zeros = false;
                }
            }
        }
    }
    if constexpr (std::totally_ordered<T>) {
        for (std::size_t i = 0; i < k; ++i) {
            if (matrix[i, i] < T::zero()) {
                matrix[i, i] = -matrix[i, i];
            }
        }
    }
    return k;
}

/// \brief Result struct for the smith algorithm
template<class T>
struct SmithFormResult {
    /// \brief Smith form of a matrix
    Matrix<T> smith_form = {};

    /// \brief Number of non zero rows or columns
    std::size_t non_empty = 0;
};

/// \brief Transforms a matrix into a Smith form
///
/// Transforms a matrix into a Smith form and returns
/// the smaller of non-zero rows or columns
///
/// \param matrix Matrix to be transformed
///
/// \return A struct containing two fields
/// 1. smith_form The transformed matrix
/// 2. non_empty The number of non-zero rows or columns
template<EuclideanDomain T>
constexpr SmithFormResult<T> smith_form(Matrix<T> matrix) {
    auto k = smith_form(std::in_place, matrix);
    return SmithFormResult {.smith_form = matrix, .non_empty = k};
}

} // namespace algebra
