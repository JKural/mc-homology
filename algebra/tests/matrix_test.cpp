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
