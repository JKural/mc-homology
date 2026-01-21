#include <algebra/algebraic_concepts.h>
#include <gtest/gtest.h>

namespace {

struct DummyGroup {
    int data = 0;

    DummyGroup& operator+=(DummyGroup y) {
        data += y.data;
        return *this;
    }

    DummyGroup& operator-=(DummyGroup y) {
        data -= y.data;
        return *this;
    }

    bool operator==(DummyGroup const&) const = default;

    static DummyGroup zero() noexcept {
        return DummyGroup {.data = 0};
    }
};

[[maybe_unused]]
DummyGroup operator+(DummyGroup x, DummyGroup y) {
    return DummyGroup {.data = x.data + y.data};
}

[[maybe_unused]]
DummyGroup operator-(DummyGroup x, DummyGroup y) {
    return DummyGroup {.data = x.data - y.data};
}

[[maybe_unused]]
DummyGroup operator+(DummyGroup x) {
    return DummyGroup {.data = +x.data};
}

[[maybe_unused]]
DummyGroup operator-(DummyGroup x) {
    return DummyGroup {.data = -x.data};
}

struct DummyRing {
    int data = 0;

    DummyRing& operator+=(DummyRing y) {
        data += y.data;
        return *this;
    }

    DummyRing& operator-=(DummyRing y) {
        data -= y.data;
        return *this;
    }

    bool operator==(DummyRing const&) const = default;

    static DummyRing zero() noexcept {
        return DummyRing {.data = 0};
    }

    DummyRing& operator*=(DummyRing y) {
        data *= y.data;
        return *this;
    }

    static DummyRing one() noexcept {
        return DummyRing {.data = 1};
    }
};

[[maybe_unused]]
DummyRing operator+(DummyRing x, DummyRing y) {
    return DummyRing {.data = x.data + y.data};
}

[[maybe_unused]]
DummyRing operator-(DummyRing x, DummyRing y) {
    return DummyRing {.data = x.data - y.data};
}

[[maybe_unused]]
DummyRing operator+(DummyRing x) {
    return DummyRing {.data = +x.data};
}

[[maybe_unused]]
DummyRing operator-(DummyRing x) {
    return DummyRing {.data = -x.data};
}

[[maybe_unused]]
DummyRing operator*(DummyRing x, DummyRing y) {
    return DummyRing {.data = x.data * y.data};
}

struct DummyField {
    double data = 0;

    DummyField& operator+=(DummyField y) {
        data += y.data;
        return *this;
    }

    DummyField& operator-=(DummyField y) {
        data -= y.data;
        return *this;
    }

    bool operator==(DummyField const&) const = default;

    static DummyField zero() noexcept {
        return DummyField {.data = 0};
    }

    DummyField& operator*=(DummyField y) {
        data *= y.data;
        return *this;
    }

    int euclidean_function() const noexcept {
        return 1;
    }

    static DummyField one() noexcept {
        return DummyField {.data = 1};
    }

    DummyField& operator/=(DummyField y) {
        data /= y.data;
        return *this;
    }
};

[[maybe_unused]]
DummyField operator+(DummyField x, DummyField y) {
    return DummyField {.data = x.data + y.data};
}

[[maybe_unused]]
DummyField operator-(DummyField x, DummyField y) {
    return DummyField {.data = x.data - y.data};
}

[[maybe_unused]]
DummyField operator+(DummyField x) {
    return DummyField {.data = +x.data};
}

[[maybe_unused]]
DummyField operator-(DummyField x) {
    return DummyField {.data = -x.data};
}

[[maybe_unused]]
DummyField operator*(DummyField x, DummyField y) {
    return DummyField {.data = x.data * y.data};
}

[[maybe_unused]]
DummyField operator/(DummyField x, DummyField y) {
    return DummyField {.data = x.data / y.data};
}

} // namespace

template<>
constexpr bool algebra::is_commutative_v<DummyGroup> = true;

template<>
constexpr bool algebra::is_commutative_v<DummyRing> = true;

template<>
constexpr bool algebra::is_commutative_v<DummyField> = true;

TEST(AlgebraicConceptsTest, AdditiveGroup) {
    using namespace algebra;

    EXPECT_FALSE(AdditiveGroup<void>);
    EXPECT_TRUE(AdditiveGroup<DummyGroup>);
    EXPECT_TRUE(AdditiveGroup<DummyRing>);
    EXPECT_TRUE(AdditiveGroup<DummyField>);
}

TEST(AlgebraicConceptsTest, AbelianRing) {
    using namespace algebra;

    EXPECT_FALSE(CommutativeRing<void>);
    EXPECT_FALSE(CommutativeRing<DummyGroup>);
    EXPECT_TRUE(CommutativeRing<DummyRing>);
    EXPECT_TRUE(CommutativeRing<DummyField>);
}

TEST(AlgebraicConceptsTest, Field) {
    using namespace algebra;

    EXPECT_FALSE(Field<void>);
    EXPECT_FALSE(Field<DummyGroup>);
    EXPECT_FALSE(Field<DummyRing>);
    EXPECT_TRUE(Field<DummyField>);
}
