#pragma once

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
            auto mult = matrix[k, j] / matrix[i, j];
            detail::submatrix_add_row(matrix, mult, k, i, j);
        }
        ++i;
    }
    return i;
}

template<class T>
struct RowEchelonFormResult {
    Matrix<T> row_echelon_form = {};
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
/// 1. row_echelon_form: The transformed matrix
/// 2. non_empty_rows: the number of non-zero rows
template<Field T>
constexpr RowEchelonFormResult<T> row_echelon_form(Matrix<T> matrix) {
    auto i = row_echelon_form(std::in_place, matrix);
    return RowEchelonFormResult {
        .row_echelon_form = matrix,
        .non_empty_rows = i
    };
}

} // namespace algebra
