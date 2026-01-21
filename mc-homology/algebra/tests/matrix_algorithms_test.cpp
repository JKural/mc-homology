#include <algebra/integer.h>
#include <algebra/matrix_algorithms.h>
#include <algebra/modulo_fields.h>
#include <gtest/gtest.h>

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

template<EuclideanDomain T>
bool is_smith(Matrix<T> const& matrix) {
    if (matrix.empty()) {
        return true;
    }
    for (std::size_t i = 0; i < matrix.nrows(); ++i) {
        for (std::size_t j = 0; j < matrix.ncols(); ++j) {
            if (i != j && matrix[i, j] != T::zero()) {
                return false;
            }
        }
    }
    std::size_t k = 0;
    for (; k + 1 < std::min(matrix.nrows(), matrix.ncols()); ++k) {
        if (matrix[k, k] == T::zero()) {
            break;
        }
        if (divide(matrix[k + 1, k + 1], matrix[k, k]).remainder != T::zero()) {
            return false;
        }
    }
    for (; k + 1 < std::min(matrix.nrows(), matrix.ncols()); ++k) {
        if (matrix[k, k] != T::zero()) {
            return false;
        }
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

TEST(MatrixAlgorithmsTest, SmithId) {
    using Matrix = Matrix<Integer>;
    Matrix m = Matrix::id(5);
    EXPECT_PRED1(is_smith<Integer>, m);
    auto [m_smith, m_non_empty] = smith_form(m);
    EXPECT_EQ(m_smith, m);
    EXPECT_EQ(m_non_empty, 5);
}

TEST(MatrixAlgorithmsTest, SmithZero) {
    using Matrix = Matrix<Integer>;
    Matrix m1 = Matrix::zero(2, 5);
    Matrix m2 = Matrix::zero(4, 4);
    Matrix m3 = Matrix::zero(6, 3);
    EXPECT_PRED1(is_smith<Integer>, m1);
    EXPECT_PRED1(is_smith<Integer>, m2);
    EXPECT_PRED1(is_smith<Integer>, m3);

    auto [m1_smith_form, m1_non_empty] = smith_form(m1);
    auto [m2_smith_form, m2_non_empty] = smith_form(m2);
    auto [m3_smith_form, m3_non_empty] = smith_form(m3);
    EXPECT_EQ(m1_smith_form, m1);
    EXPECT_EQ(m1_non_empty, 0);
    EXPECT_EQ(m2_smith_form, m2);
    EXPECT_EQ(m2_non_empty, 0);
    EXPECT_EQ(m3_smith_form, m3);
    EXPECT_EQ(m3_non_empty, 0);
}

TEST(MatrixAlgorithmsTest, Smith) {
    using Matrix = Matrix<Integer>;
    // clang-format off
    Matrix m1(
        std::vector {2, 0, 3, 2,
                     1, 5, 3, 0},
        2, 4);
    Matrix m2(
        std::vector {2,  8, -4, 12,
                     4, 16,  6, 10,
                     2,  8,  3,  5,
                     0,  3,  0,  3},
        4, 4);
    // clang-format on
    Matrix m3 = m1.transpose();
    Matrix m4 = m2.transpose();

    // clang-format off
    Matrix m1_expected(
        std::vector {1, 0, 0, 0,
                     0, 1, 0, 0},
        2, 4);
    Matrix m2_expected(
        std::vector {2, 0, 0, 0,
                     0, 3, 0, 0,
                     0, 0, 7, 0,
                     0, 0, 0, 0},
        4, 4);
    Matrix m3_expected = m1_expected.transpose();
    Matrix m4_expected = m2_expected.transpose();
    // clang-format on

    auto m1_result = smith_form(m1);
    auto m2_result = smith_form(m2);
    auto m3_result = smith_form(m3);
    auto m4_result = smith_form(m4);
    EXPECT_EQ(m1_result.smith_form, m1_expected);
    EXPECT_EQ(m1_result.non_empty, 2);
    EXPECT_EQ(m2_result.smith_form, m2_expected);
    EXPECT_EQ(m2_result.non_empty, 3);
    EXPECT_EQ(m3_result.smith_form, m3_expected);
    EXPECT_EQ(m3_result.non_empty, 2);
    EXPECT_EQ(m4_result.smith_form, m4_expected);
    EXPECT_EQ(m4_result.non_empty, 3);
}
