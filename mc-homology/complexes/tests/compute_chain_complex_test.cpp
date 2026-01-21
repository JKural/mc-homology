#include <algebra/integer.h>
#include <complexes/compute_chain_complex.h>
#include <complexes/cubical_complex.h>
#include <gtest/gtest.h>

#include <vector>

#include "algebra/chain_complex.h"

using namespace complexes;

TEST(ComputeChainComplex, Points) {
    auto p0 = CubicalSimplex::point(0);
    auto p1 = CubicalSimplex::point(1);
    CubicalComplex c1;
    CubicalComplex c2;
    CubicalComplex c3;
    c2.add(p0);
    c3.add(p0);
    c3.add(p1);
    auto chain1 = compute_chain_complex<algebra::Integer>(c1);
    auto chain2 = compute_chain_complex<algebra::Integer>(c2);
    auto chain3 = compute_chain_complex<algebra::Integer>(c3);
    auto hom1 = algebra::homology(chain1);
    auto hom2 = algebra::homology(chain2);
    auto hom3 = algebra::homology(chain3);
    auto tor1 = std::vector<std::vector<algebra::Integer>> {};
    auto tor2 = std::vector<std::vector<algebra::Integer>> {{}};
    auto tor3 = std::vector<std::vector<algebra::Integer>> {{}};
    auto bt1 = std::vector<std::size_t> {};
    auto bt2 = std::vector<std::size_t> {1};
    auto bt3 = std::vector<std::size_t> {2};
    // EXPECT_EQ(hom1.betti_numbers, bt1);
    // EXPECT_EQ(hom2.betti_numbers, bt2);
    // EXPECT_EQ(hom3.betti_numbers, bt3);
    EXPECT_EQ(hom1.torsion, tor1);
    EXPECT_EQ(hom2.torsion, tor2);
    EXPECT_EQ(hom3.torsion, tor3);
}

TEST(ComputeChainComplex, Circle) {
    CubicalComplex thin_circle {};
    thin_circle.add_recursive(
        product(CubicalSimplex::interval(0), CubicalSimplex::interval(0))
    );
    thin_circle.remove(
        product(CubicalSimplex::interval(0), CubicalSimplex::interval(0))
    );
    CubicalComplex thick_circle {};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            thick_circle.add_recursive(product(
                CubicalSimplex::interval(i),
                CubicalSimplex::interval(j)
            ));
        }
    }
    thick_circle.remove(
        product(CubicalSimplex::interval(1), CubicalSimplex::interval(1))
    );
    auto chain_thin = compute_chain_complex<algebra::Integer>(thin_circle);
    auto chain_thick = compute_chain_complex<algebra::Integer>(thick_circle);
    auto hom_thin = algebra::homology(chain_thin);
    auto hom_thick = algebra::homology(chain_thick);
    std::vector<std::size_t> expected_betti_thin {1, 1};
    std::vector<std::size_t> expected_betti_thick {1, 1, 0};
    std::vector<std::vector<algebra::Integer>> expected_torsion_thin {
        {},
        {},
    };
    std::vector<std::vector<algebra::Integer>> expected_torsion_thick {
        {},
        {},
        {},
    };
    EXPECT_EQ(hom_thin.betti_numbers, expected_betti_thin);
    EXPECT_EQ(hom_thick.betti_numbers, expected_betti_thick);
    EXPECT_EQ(hom_thin.torsion, expected_torsion_thin);
    EXPECT_EQ(hom_thick.torsion, expected_torsion_thick);
}

TEST(ComputeChainComplex, Sphere) {
    CubicalComplex sphere_thin;
    sphere_thin.add_recursive(product(
        product(CubicalSimplex::interval(0), CubicalSimplex::interval(0)),
        CubicalSimplex::interval(0)
    ));
    sphere_thin.remove(product(
        product(CubicalSimplex::interval(0), CubicalSimplex::interval(0)),
        CubicalSimplex::interval(0)
    ));
    CubicalComplex sphere_thick;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                sphere_thick.add_recursive(product(
                    product(
                        CubicalSimplex::interval(i),
                        CubicalSimplex::interval(j)
                    ),
                    CubicalSimplex::interval(k)
                ));
            }
        }
    }
    sphere_thick.remove(product(
        product(CubicalSimplex::interval(1), CubicalSimplex::interval(1)),
        CubicalSimplex::interval(1)
    ));
    auto chain_thin = compute_chain_complex<algebra::Integer>(sphere_thin);
    auto chain_thick = compute_chain_complex<algebra::Integer>(sphere_thick);
    auto hom_thin = algebra::homology(chain_thin);
    auto hom_thick = algebra::homology(chain_thick);
    std::vector<std::size_t> expected_betti_thin {1, 0, 1};
    std::vector<std::size_t> expected_betti_thick {1, 0, 1, 0};
    std::vector<std::vector<algebra::Integer>> expected_torsion_thin {
        {},
        {},
        {},
    };
    std::vector<std::vector<algebra::Integer>> expected_torsion_thick {
        {},
        {},
        {},
        {},
    };
    EXPECT_EQ(hom_thin.betti_numbers, expected_betti_thin);
    EXPECT_EQ(hom_thick.betti_numbers, expected_betti_thick);
    EXPECT_EQ(hom_thin.torsion, expected_torsion_thin);
    EXPECT_EQ(hom_thick.torsion, expected_torsion_thick);
}
