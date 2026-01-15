#include "matrix.h"

#include <gtest/gtest.h>

#include <array>
#include <stdexcept>

#include "z2_field.h"

using namespace algebra;

TEST(MatrixTest, Creation) {
    std::vector<Z2> data1 = {0, 1, 1, 0};
    std::array<ZModP<3>, 9> data2 = {0, 1, 2, 1, 0, 2, 2, 1, 0};
    Matrix matrix1(std::move(data1), 2, 2);
    EXPECT_EQ((matrix1[0, 0]), 0);
    EXPECT_EQ((matrix1[0, 1]), 1);
    EXPECT_EQ((matrix1[1, 0]), 1);
    EXPECT_EQ((matrix1[1, 1]), 0);
    EXPECT_TRUE(data1.empty());

    Matrix matrix2(data2, 3, 3);
    EXPECT_EQ((matrix2[0, 0]), 0);
    EXPECT_EQ((matrix2[0, 1]), 1);
    EXPECT_EQ((matrix2[0, 2]), 2);
    EXPECT_EQ((matrix2[1, 0]), 1);
    EXPECT_EQ((matrix2[1, 1]), 0);
    EXPECT_EQ((matrix2[1, 2]), 2);
    EXPECT_EQ((matrix2[2, 0]), 2);
    EXPECT_EQ((matrix2[2, 1]), 1);
    EXPECT_EQ((matrix2[2, 2]), 0);

    EXPECT_THROW(Matrix(data2, 2, 4), std::domain_error);
}

TEST(MatrixTest, IdentityAndZero) {
    auto matrix1 = Matrix<Z2>::id(2);
    EXPECT_EQ((matrix1.at(0, 0)), 1);
    EXPECT_EQ((matrix1.at(0, 1)), 0);
    EXPECT_EQ((matrix1.at(1, 0)), 0);
    EXPECT_EQ((matrix1.at(1, 1)), 1);
    auto matrix2 = Matrix<ZModP<3>>::zero(2, 4);
    for (auto x : matrix2) {
        EXPECT_EQ(x, ZModP<3>::zero());
    }
    EXPECT_EQ(matrix2.nrows(), 2);
    EXPECT_EQ(matrix2.ncols(), 4);
}

TEST(MatrixTest, Printing) {
    using Z13 = ZModP<13>;
    using Matrix = Matrix<Z13>;
    // clang-format off
    Matrix m(
        std::vector {1, 2, 3, 4,
                     5, 6, 7, 8},
        2, 4);
    // clang-format on
    std::stringstream sstream;
    sstream << m;
    std::string txt1 = sstream.str();
    std::string txt2 = std::format("{}", m);
    std::string txt3 = std::format("{:-}", m);
    std::string txt4 = std::format("{:#}", m);
    std::string txt5 = std::format("{::-^3}", m);
    std::string txt6 = std::format("{:#:-^3}", m);
    std::string txt1_expected = "[[1, 2, 3, 4], [5, 6, 7, 8]]";
    std::string txt2_expected = "[[1, 2, 3, 4], [5, 6, 7, 8]]";
    std::string txt3_expected = "[[1, 2, 3, 4], [5, 6, 7, 8]]";
    std::string txt4_expected =
        "[[1, 2, 3, 4],\n [5, 6, 7, 8]]\nMatrix 2 x 4\n";
    std::string txt5_expected = "[[-1-, -2-, -3-, -4-], [-5-, -6-, -7-, -8-]]";
    std::string txt6_expected =
        "[[-1-, -2-, -3-, -4-],\n [-5-, -6-, -7-, -8-]]\nMatrix 2 x 4\n";
    EXPECT_EQ(txt1, txt1_expected);
    EXPECT_EQ(txt2, txt2_expected);
    EXPECT_EQ(txt3, txt3_expected);
    EXPECT_EQ(txt4, txt4_expected);
    EXPECT_EQ(txt5, txt5_expected);
    EXPECT_EQ(txt6, txt6_expected);
}

TEST(MatrixTest, Transpose) {
    using Z5 = ZModP<5>;
    using Matrix = Matrix<Z5>;
    // clang-format off
    Matrix m(
        std::vector {1, 2, 3,
                     4, 5, 6},
        2, 3);
    // clang-format on
    auto m_t = m.transpose();
    EXPECT_EQ(m_t.nrows(), 3);
    EXPECT_EQ(m_t.ncols(), 2);
    for (std::size_t i = 0; i < 2; ++i) {
        for (std::size_t j = 0; j < 3; ++j) {
            EXPECT_EQ(m_t.at(j, i), m.at(i, j));
        }
    }
}
