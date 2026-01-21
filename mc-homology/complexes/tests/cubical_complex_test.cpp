#include <complexes/cubical_complex.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

using namespace complexes;

TEST(BasicIntervalTest, Basics) {
    auto a = BasicInterval::point(0);
    auto b = BasicInterval::point(1);
    auto ab = BasicInterval::interval(0);
    EXPECT_EQ(a.left(), 0);
    EXPECT_EQ(a.right(), 0);
    EXPECT_TRUE(a.is_trivial());
    EXPECT_EQ(b.left(), 1);
    EXPECT_EQ(b.right(), 1);
    EXPECT_TRUE(b.is_trivial());
    EXPECT_EQ(ab.left(), 0);
    EXPECT_EQ(ab.right(), 1);
    EXPECT_FALSE(ab.is_trivial());
}

TEST(BasicIntervalTest, Printing) {
    std::stringstream sstream1;
    std::stringstream sstream2;
    BasicInterval i1 = BasicInterval::point(0);
    BasicInterval i2 = BasicInterval::interval(0);
    sstream1 << i1;
    sstream2 << i2;
    EXPECT_EQ(sstream1.str(), "[0]");
    EXPECT_EQ(sstream2.str(), "[0, 1]");
    EXPECT_EQ(std::format("{}", i1), "[0]");
    EXPECT_EQ(std::format("{}", i2), "[0, 1]");
    EXPECT_EQ(std::format("{:^3}", i1), "[ 0 ]");
    EXPECT_EQ(std::format("{:^3}", i2), "[ 0 ,  1 ]");
}

TEST(BasicIntervalTest, Hash) {
    auto a = BasicInterval::point(0);
    auto b = BasicInterval::point(1);
    auto a2 = BasicInterval::point(0);
    auto ab = BasicInterval::interval(0);
    EXPECT_EQ(a.hash(), a2.hash());
    EXPECT_NE(a.hash(), b.hash());
    EXPECT_NE(a.hash(), ab.hash());
}

TEST(CubicalSimplexTest, Printing) {
    CubicalSimplex point = CubicalSimplex::point(0);
    CubicalSimplex line = CubicalSimplex::interval(0);
    CubicalSimplex square = product(line, line);
    std::stringstream sstream1;
    std::stringstream sstream2;
    std::stringstream sstream3;
    sstream1 << point;
    sstream2 << line;
    sstream3 << square;
    EXPECT_EQ(sstream1.str(), "[0]");
    EXPECT_EQ(sstream2.str(), "[0, 1]");
    EXPECT_EQ(sstream3.str(), "[0, 1]x[0, 1]");
    EXPECT_EQ(std::format("{:*^5}", point), "[**0**]");
    EXPECT_EQ(std::format("{:*^5}", line), "[**0**, **1**]");
    EXPECT_EQ(std::format("{:*^5}", square), "[**0**, **1**]x[**0**, **1**]");
}

TEST(CubicalSimplexTest, Boundary) {
    CubicalSimplex point = CubicalSimplex::point(0);
    CubicalSimplex line = CubicalSimplex::interval(0);
    CubicalSimplex square = product(line, line);
    std::vector<CubicalSimplex> expected_boundary_point {};
    std::vector expected_boundary_line {
        CubicalSimplex::point(1),
        CubicalSimplex::point(0)
    };
    std::vector expected_boundary_square {
        product(CubicalSimplex::point(1), CubicalSimplex::interval(0)),
        product(CubicalSimplex::point(0), CubicalSimplex::interval(0)),
        product(CubicalSimplex::interval(0), CubicalSimplex::point(1)),
        product(CubicalSimplex::interval(0), CubicalSimplex::point(0)),
    };
    EXPECT_EQ(point.boundary(), expected_boundary_point);
    EXPECT_PRED2(std::ranges::is_sorted, point.boundary(), [](auto x, auto y) {
        return x > y;
    });
    EXPECT_EQ(line.boundary(), expected_boundary_line);
    EXPECT_PRED2(std::ranges::is_sorted, line.boundary(), [](auto x, auto y) {
        return x > y;
    });
    EXPECT_EQ(square.boundary(), expected_boundary_square);
    EXPECT_PRED2(std::ranges::is_sorted, square.boundary(), [](auto x, auto y) {
        return x > y;
    });
}

TEST(CubicalSimplexTest, Product) {
    auto p = CubicalSimplex::point(0);
    auto l = CubicalSimplex::interval(0);
    auto pp = product(p, p);
    auto pl = product(p, l);
    auto lp = product(l, p);
    auto ll = product(l, l);
    EXPECT_EQ(p.dimension(), 0);
    EXPECT_EQ(p.ambient_dimension(), 1);
    EXPECT_EQ(pp.dimension(), 0);
    EXPECT_EQ(pp.ambient_dimension(), 2);
    EXPECT_EQ(l.dimension(), 1);
    EXPECT_EQ(l.ambient_dimension(), 1);
    EXPECT_EQ(pl.dimension(), 1);
    EXPECT_EQ(pl.ambient_dimension(), 2);
    EXPECT_EQ(lp.dimension(), 1);
    EXPECT_EQ(lp.ambient_dimension(), 2);
    EXPECT_EQ(ll.dimension(), 2);
    EXPECT_EQ(ll.ambient_dimension(), 2);
    EXPECT_NE(pl, lp);
}

TEST(CubicalComplexTest, Basics) {
    CubicalComplex complex1;
    CubicalComplex complex2;

    auto p0 = CubicalSimplex::point(0);
    auto p1 = CubicalSimplex::point(1);
    auto p00 = product(p0, p0);
    auto p01 = product(p0, p1);
    auto p10 = product(p1, p0);
    auto p11 = product(p1, p1);
    auto l01 = CubicalSimplex::interval(0);
    auto l0001 = product(p0, l01);
    auto l1011 = product(p1, l01);
    auto l0010 = product(l01, p0);
    auto l0111 = product(l01, p1);
    auto sq = product(l01, l01);
    complex1.add(p00);
    complex1.add(p01);
    complex1.add(p10);
    complex1.add(p11);
    complex1.add(l0001);
    complex1.add(l1011);
    complex1.add(l0010);
    complex1.add(l0111);
    auto complex3 = complex1;
    complex1.add(sq);

    complex2.add_recursive(sq);

    auto simplices = std::vector {
        std::unordered_set {p00, p01, p10, p11},
        std::unordered_set {l0001, l1011, l0010, l0111},
        std::unordered_set {sq},
    };

    EXPECT_EQ(complex1, complex2);
    EXPECT_EQ(complex1.simplices(), simplices);

    complex2.remove(sq);
    simplices.pop_back();

    EXPECT_EQ(complex2, complex3);
    EXPECT_EQ(complex3.simplices(), simplices);
}
