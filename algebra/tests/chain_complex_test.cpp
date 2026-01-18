#include "chain_complex.h"

#include <gtest/gtest.h>

#include <ranges>
#include <vector>

#include "integer.h"
#include "matrix.h"
#include "z2_field.h"

using namespace algebra;
namespace vs = std::views;

namespace {

template<class T>
ChainComplex<T> points(std::size_t n) {
    return ChainComplex<T> {std::vector {Matrix<T>::zero(0, n)}};
}

template<class T>
ChainComplex<T> points_reduced(std::size_t n) {
    return ChainComplex<T> {
        std::vector {Matrix<T>(vs::repeat(T::one(), n), 1, n)}
    };
}

template<class T>
ChainComplex<T> n_sphere(std::size_t n) {
    std::vector<Matrix<T>> boundaries(n + 1);
    switch (n) {
        case 0:
            boundaries[0] = Matrix<T>::zero(0, 2);
            break;
        case 1:
            boundaries[0] = Matrix<T>::zero(0, 1);
            boundaries[1] = Matrix<T>::zero(1, 1);
            break;
        default:
            boundaries[0] = Matrix<T>::zero(0, 1);
            boundaries[1] = Matrix<T>::zero(1, 0);
            boundaries[n] = Matrix<T>::zero(0, 1);
            break;
    }
    return ChainComplex {std::move(boundaries)};
}

template<class T>
ChainComplex<T> klein_bottle() {
    return ChainComplex {std::vector<Matrix<T>> {
        Matrix<T>::zero(0, 1),
        Matrix<T>::zero(1, 2),
        Matrix<T>(std::vector<T> {2, 0}, 2, 1)
    }};
}

} // namespace

TEST(ChainComplexTest, Points) {
    auto homology_point_z2 = homology(points<Z2>(1));
    auto homology_point_z2_reduced = homology(points_reduced<Z2>(1));
    EXPECT_EQ(homology_point_z2.betti_numbers, std::vector<std::size_t> {1});
    EXPECT_EQ(homology_point_z2.torsion, std::vector<std::vector<Z2>> {{}});
    EXPECT_EQ(
        homology_point_z2_reduced.betti_numbers,
        std::vector<std::size_t> {0}
    );
    EXPECT_EQ(
        homology_point_z2_reduced.torsion,
        std::vector<std::vector<Z2>>(1)
    );

    auto homology_points_z2 = homology(points<Z2>(9));
    auto homology_points_z2_reduced = homology(points_reduced<Z2>(9));
    EXPECT_EQ(homology_points_z2.betti_numbers, std::vector<std::size_t> {9});
    EXPECT_EQ(homology_points_z2.torsion, std::vector<std::vector<Z2>> {{}});
    EXPECT_EQ(
        homology_points_z2_reduced.betti_numbers,
        std::vector<std::size_t> {8}
    );
    EXPECT_EQ(
        homology_points_z2_reduced.torsion,
        std::vector<std::vector<Z2>> {{}}
    );

    auto homology_point_z = homology(points<Integer>(1));
    auto homology_point_z_reduced = homology(points_reduced<Integer>(1));
    EXPECT_EQ(homology_point_z.betti_numbers, std::vector<std::size_t> {1});
    EXPECT_EQ(homology_point_z.torsion, std::vector<std::vector<Integer>> {{}});
    EXPECT_EQ(
        homology_point_z_reduced.betti_numbers,
        std::vector<std::size_t> {0}
    );
    EXPECT_EQ(
        homology_point_z_reduced.torsion,
        std::vector<std::vector<Integer>> {{}}
    );

    auto homology_points_z = homology(points<Integer>(9));
    auto homology_points_z_reduced = homology(points_reduced<Integer>(9));
    EXPECT_EQ(homology_points_z.betti_numbers, std::vector<std::size_t> {9});
    EXPECT_EQ(
        homology_points_z.torsion,
        std::vector<std::vector<Integer>> {{}}
    );
    EXPECT_EQ(
        homology_points_z_reduced.betti_numbers,
        std::vector<std::size_t> {8}
    );
    EXPECT_EQ(
        homology_points_z_reduced.torsion,
        std::vector<std::vector<Integer>> {{}}
    );
}

TEST(ChainComplexTest, Sphere) {
    auto homology_0_sphere_z = homology(n_sphere<Integer>(0));
    auto homology_1_sphere_z = homology(n_sphere<Integer>(1));
    auto homology_2_sphere_z = homology(n_sphere<Integer>(2));
    auto homology_9_sphere_z = homology(n_sphere<Integer>(9));

    auto const expected_betti_numbers = [](std::size_t n) {
        if (n == 0) {
            return std::vector<std::size_t> {2};
        } else {
            auto betti = std::vector<std::size_t>(n + 1);
            betti[0] = 1;
            betti[n] = 1;
            return betti;
        }
    };

    auto const expected_torsion = [](std::size_t n, auto x) {
        return std::vector<std::vector<decltype(x)>>(n + 1);
    };

    EXPECT_EQ(homology_0_sphere_z.betti_numbers, expected_betti_numbers(0));
    EXPECT_EQ(homology_0_sphere_z.torsion, expected_torsion(0, Integer {}));
    EXPECT_EQ(homology_1_sphere_z.betti_numbers, expected_betti_numbers(1));
    EXPECT_EQ(homology_1_sphere_z.torsion, expected_torsion(1, Integer {}));
    EXPECT_EQ(homology_2_sphere_z.betti_numbers, expected_betti_numbers(2));
    EXPECT_EQ(homology_2_sphere_z.torsion, expected_torsion(2, Integer {}));
    EXPECT_EQ(homology_9_sphere_z.betti_numbers, expected_betti_numbers(9));
    EXPECT_EQ(homology_9_sphere_z.torsion, expected_torsion(9, Integer {}));
}

TEST(ChainComplexTest, KleinBottle) {
    auto homology_klein_bottle_z = homology(klein_bottle<Integer>());
    auto homology_klein_bottle_z2 = homology(klein_bottle<Z2>());
    auto homology_klein_bottle_z3 = homology(klein_bottle<ZModP<3>>());

    auto homology_expected_z = Homology<Integer> {
        .betti_numbers = {1, 1, 0},
        .torsion = {{}, {Integer(2)}, {}}
    };

    auto homology_expected_z2 =
        Homology<Z2> {.betti_numbers = {1, 2, 1}, .torsion = {{}, {}, {}}};

    auto homology_expected_z3 = Homology<ZModP<3>> {
        .betti_numbers = {1, 1, 0},
        .torsion = {{}, {}, {}}
    };

    EXPECT_EQ(
        homology_klein_bottle_z.betti_numbers,
        homology_expected_z.betti_numbers
    );
    EXPECT_EQ(homology_klein_bottle_z.torsion, homology_expected_z.torsion);
    EXPECT_EQ(
        homology_klein_bottle_z2.betti_numbers,
        homology_expected_z2.betti_numbers
    );
    EXPECT_EQ(homology_klein_bottle_z2.torsion, homology_expected_z2.torsion);
    EXPECT_EQ(
        homology_klein_bottle_z3.betti_numbers,
        homology_expected_z3.betti_numbers
    );
    EXPECT_EQ(homology_klein_bottle_z3.torsion, homology_expected_z3.torsion);
}
