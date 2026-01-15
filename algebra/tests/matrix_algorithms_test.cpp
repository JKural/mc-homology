#include "matrix_algorithms.h"

#include <gtest/gtest.h>

#include "gtest/gtest.h"
#include "modulo_fields.h"

using namespace algebra;

namespace {

template<Field T>
bool is_row_echelon(Matrix<T> const& matrix) {
    if (matrix.empty()) {
        return true;
    }
    std::size_t last_col = 0;
    while (last_col < matrix.ncols() && matrix[0, last_col] == T::zero()) {
        ++last_col;
    }
    for (std::size_t row = 1; row < matrix.nrows(); ++row) {
        std::size_t col = 0;
        while (col < matrix.ncols() && matrix[row, col] == T::zero()) {
            ++col;
        }
        if (col <= last_col && col != matrix.ncols()) {
            return false;
        }
        last_col = col;
    }
    return true;
}

} // namespace

TEST(MatrixAlgorithmsTest, RowEchelonId) {
    using Z7 = ZModP<7>;
    using Matrix = Matrix<Z7>;
    Matrix m = Matrix::id(5);
    EXPECT_PRED1(is_row_echelon<Z7>, m);
    auto [m_row_echelon, m_non_empty_rows] = row_echelon_form(m);
    EXPECT_EQ(m_row_echelon, m);
    EXPECT_EQ(m_non_empty_rows, 5);
}

TEST(MatrixAlgorithmsTest, RowEchelonZero) {
    using Z11 = ZModP<11>;
    using Matrix = Matrix<Z11>;
    Matrix m1 = Matrix::zero(2, 5);
    Matrix m2 = Matrix::zero(4, 4);
    Matrix m3 = Matrix::zero(6, 3);
    EXPECT_PRED1(is_row_echelon<Z11>, m1);
    EXPECT_PRED1(is_row_echelon<Z11>, m2);
    EXPECT_PRED1(is_row_echelon<Z11>, m3);

    auto [m1_row_echelon, m1_non_empty_rows] = row_echelon_form(m1);
    auto [m2_row_echelon, m2_non_empty_rows] = row_echelon_form(m2);
    auto [m3_row_echelon, m3_non_empty_rows] = row_echelon_form(m3);
    EXPECT_EQ(m1_row_echelon, m1);
    EXPECT_EQ(m1_non_empty_rows, 0);
    EXPECT_EQ(m2_row_echelon, m2);
    EXPECT_EQ(m2_non_empty_rows, 0);
    EXPECT_EQ(m3_row_echelon, m3);
    EXPECT_EQ(m3_non_empty_rows, 0);
}

TEST(MatrixAlgorithmsTest, RowEchelon) {
    using Z13 = ZModP<13>;
    using Matrix = Matrix<Z13>;
    // clang-format off
    Matrix m1(
        std::vector {2, 0, 3, 2,
                     1, 5, 3, 0},
        2, 4);
    // clang-format on
    auto m1_row_echelon_result = row_echelon_form(m1);
    EXPECT_PRED1(is_row_echelon<Z13>, m1_row_echelon_result.row_echelon_form);
    EXPECT_EQ(m1_row_echelon_result.non_empty_rows, 2);

    auto m2 = m1.transpose();
    auto m2_row_echelon_result = row_echelon_form(m2);
    EXPECT_PRED1(is_row_echelon<Z13>, m2_row_echelon_result.row_echelon_form);
    EXPECT_EQ(m2_row_echelon_result.non_empty_rows, 2);
}
