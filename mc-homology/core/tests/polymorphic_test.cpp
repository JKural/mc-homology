#include <core/polymorphic.h>
#include <gtest/gtest.h>

#include <utility>

using namespace core;

namespace {

struct Base {
    virtual int foo() const = 0;
    virtual ~Base() = default;
};

struct Derived1: public Base {
    int foo() const override {
        return 1;
    }
};

struct Derived2: public Base {
    int foo() const override {
        return 2;
    }
};

struct Derived3: public Derived2 {
    Derived3(int x) : x(x) {}

    int foo() const override {
        return x;
    }

    int x = 0;
};

} // namespace

TEST(PolymorphicTest, Constructors) {
    Polymorphic<Base> p1 {Derived1 {}};
    Polymorphic p1_copy {p1};
    EXPECT_EQ(p1->foo(), 1);
    EXPECT_EQ(p1_copy->foo(), 1);
    Polymorphic<Derived2> p2 {Derived2 {}};
    EXPECT_EQ(p2->foo(), 2);
    p2 = Polymorphic<Derived2> {std::in_place_type<Derived3>, 5};
    EXPECT_EQ(p2->foo(), 5);
    auto p3 = std::move(p2);
    EXPECT_EQ(p3->foo(), 5);
}

TEST(PolymorphicTest, ValuelessAfterMove) {
    Polymorphic<Base> p1 {std::in_place_type<Derived3>, 5};
    Polymorphic<Base> p2 {Derived1 {}};
    EXPECT_EQ(p1->foo(), 5);
    EXPECT_EQ(p2->foo(), 1);
    p2 = std::move(p1);
    EXPECT_TRUE(p1.valueless_after_move());
    EXPECT_EQ(p2->foo(), 5);
}
