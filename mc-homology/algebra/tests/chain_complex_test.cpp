#include "algebra/chain_complex.h"

#include <gtest/gtest.h>

#include <algorithm>
#include <ranges>
#include <vector>

#include "algebra/integer.h"
#include "algebra/matrix.h"
#include "algebra/z2_field.h"

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

TEST(ChainComplexTest, BigSimplex) {
    namespace rs = std::ranges;
    namespace vs = std::views;
    auto n_choose_k = [cache = std::vector<std::vector<std::size_t>> {
                           {1}
                       }](std::size_t n, std::size_t k) mutable {
        for (std::size_t i = cache.size(); i <= n; ++i) {
            cache.emplace_back(i + 1, 1);
            for (std::size_t j = 1; j < i; ++j) {
                cache[i][j] = cache[i - 1][j - 1] + cache[i - 1][j];
            }
        }
        return cache[n][k];
    };
    auto subsets = [&n_choose_k](std::size_t n, std::size_t k) {
        std::vector<std::vector<std::size_t>> indexes(n_choose_k(n, k));
        indexes[0] = rs::to<std::vector<std::size_t>>(vs::iota(0u, k));
        for (std::size_t i = 1; i < indexes.size(); ++i) {
            indexes[i] = indexes[i - 1];
            std::size_t j = k;
            for (; j > 0; --j) {
                if (indexes[i][j - 1] < n - k + j - 1) {
                    ++indexes[i][j - 1];
                    break;
                }
            }
            if (j == 0) {
                continue;
            }
            for (; j < k; ++j) {
                indexes[i][j] = indexes[i][j - 1] + 1;
            }
        }
        return indexes;
    };
    auto const simplex = [&n_choose_k, &subsets](std::size_t n) {
        std::vector<Matrix<Integer>> boundaries(n + 1);
        auto const subset = [](auto const& range1, auto const& range2) {
            auto first1 = rs::begin(range1);
            auto first2 = rs::begin(range2);
            auto const last1 = rs::end(range1);
            auto const last2 = rs::end(range2);
            for (; first1 != last1; ++first1) {
                while (first2 != last2 && *first1 > *first2) {
                    ++first2;
                }
                if (first2 == last2 || *first1 != *first2) {
                    return false;
                }
                ++first2;
            }
            return true;
        };
        for (std::size_t d = 0; d <= n; ++d) {
            boundaries[d] = Matrix<Integer>::zero(
                n_choose_k(n + 1, d),
                n_choose_k(n + 1, d + 1)
            );
            for (auto&& [j, subset_col] :
                 subsets(n + 1, d + 1) | vs::enumerate) {
                bool negate = false;
                for (auto&& [i, subset_row] :
                     subsets(n + 1, d) | vs::enumerate) {
                    if (subset(subset_row, subset_col)) {
                        boundaries[d][i, j] = negate ? -1 : 1;
                        negate = !negate;
                    }
                }
            }
        }
        return ChainComplex<Integer> {std::move(boundaries)};
    };

    auto homology_simplex_0 = homology(simplex(0));
    auto homology_simplex_1 = homology(simplex(1));
    auto homology_simplex_2 = homology(simplex(2));
    auto homology_simplex_4 = homology(simplex(4));
    auto homology_simplex_8 = homology(simplex(8));
    EXPECT_EQ(homology_simplex_0.betti_numbers, std::vector<std::size_t>(1));
    EXPECT_EQ(homology_simplex_0.torsion, std::vector<std::vector<Integer>>(1));
    EXPECT_EQ(homology_simplex_1.betti_numbers, std::vector<std::size_t>(2));
    EXPECT_EQ(homology_simplex_1.torsion, std::vector<std::vector<Integer>>(2));
    EXPECT_EQ(homology_simplex_2.betti_numbers, std::vector<std::size_t>(3));
    EXPECT_EQ(homology_simplex_2.torsion, std::vector<std::vector<Integer>>(3));
    EXPECT_EQ(homology_simplex_4.betti_numbers, std::vector<std::size_t>(5));
    EXPECT_EQ(homology_simplex_4.torsion, std::vector<std::vector<Integer>>(5));
    EXPECT_EQ(homology_simplex_8.betti_numbers, std::vector<std::size_t>(9));
    EXPECT_EQ(homology_simplex_8.torsion, std::vector<std::vector<Integer>>(9));
}
