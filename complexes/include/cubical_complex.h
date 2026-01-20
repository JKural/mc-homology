/// \file cubical_complex.h
/// \brief A file containing the CubicalComplex class

#pragma once

#include <compare>
#include <format>
#include <iostream>
#include <ranges>
#include <unordered_set>
#include <vector>

namespace complexes {

/// \brief A struct representing an interval of length 0 or 1
///
/// A struct representing an interval of a form [`left`, `right`], where
/// `right - left < 2`
class BasicInterval {
public:
    /// \brief Returns an singleton of 0
    BasicInterval();

    /// \brief Computer a hash of the inverval
    std::size_t hash() const;

    /// \brief Compares two intervals
    bool operator==(BasicInterval const&) const;

    /// \brief Returns the left end of the interval
    int left() const;

    /// \brief Returns the right end of the interval
    int right() const;

    /// \brief Returns true, if the interval is a singleton, false
    ///        otherwise
    bool is_trivial() const;

    /// \brief Creates a trivial interval
    static BasicInterval point(int p);

    /// \brief Creates a interval of length one starting in left
    ///
    /// \param left end of the interval
    static BasicInterval interval(int left);

private:
    int m_left = 0;
    bool m_full = false;
};

/// \brief Prints an interval to output
std::ostream& operator<<(std::ostream& output, BasicInterval i);

/// \brief A class representing a single cubical simplex in a complex
///
/// A cubical simplex is a product of basic intervals. The number of
/// intervals is equal to the ambient dimension, while the number of
/// non trivial intervals is equal to the dimension of the simplex.
class CubicalSimplex {
public:
    /// \brief Constructs a simplex from a vector of intervals
    ///
    /// Constructs a simplex from a vector of intervals. The interval
    /// must have at least one value.
    ///
    /// \param intervals A nonempty vector of intervals
    CubicalSimplex(std::vector<BasicInterval> intervals);

    /// \brief Returns the dimension of the simplex
    std::size_t dimension() const;

    /// \brief Returns the ambient dimension of the simplex
    std::size_t ambient_dimension() const;

    /// \brief Computes a hash of the simplex
    std::size_t hash() const;

    /// \brief Returns the boundary of a simplex
    ///
    /// The returned boundary is decresing in the sense of operator<=>.
    std::vector<CubicalSimplex> boundary() const;

    /// \brief Compares two simplices
    bool operator==(CubicalSimplex const&) const;

    /// \brief Returns the underlying intervals
    std::vector<BasicInterval> const& intervals() const;

    /// \brief Comparison operator for simplices
    ///
    /// Comparison operator for simplices. A simplex A compares
    /// smaller than simplex B if and only if it has smaller dimension
    /// or the dimensions are equal and its interval vector is
    /// lexicographically smaller with order on intervals given
    /// by:
    /// 1. `[a, a + 1] < [b]`
    /// 2. `[a, a + 1] < [b, b + 1]`
    /// 3. `[a] < [b]`
    ///
    /// for any `a < b`
    std::strong_ordering operator<=>(CubicalSimplex const& simplex) const;

    /// \brief Creates a cubical simplex out of a single 1d point
    ///
    /// \param p The point
    static CubicalSimplex point(int p);

    /// \brief Creates a cubical simplex out of a single 1d interval
    ///
    /// \param left Left bound of the interval
    static CubicalSimplex interval(int left);

    /// \brief Creates a cubical simplex by joining together two simplices
    ///
    /// Creates a simplex in a higher dimension by taking a cartesian
    /// product and concatenating them
    ///
    /// \param s1 First simplex
    /// \param s2 Second simplex
    ///
    /// \return The result of concatenation
    friend CubicalSimplex
    product(CubicalSimplex const& s1, CubicalSimplex const& s2);

private:
    CubicalSimplex();

    std::vector<BasicInterval> m_intervals = {};
    std::size_t m_dimension = 0;
};

/// \brief Prints a cubical simplex to output
std::ostream& operator<<(std::ostream& output, CubicalSimplex const& s);

class CubicalComplex {
public:
    /// \brief Default constructor for the CubicalComplex
    ///
    /// Creates an empty complex
    CubicalComplex();

    /// \brief Adds a cubical simplex to the complex
    ///
    /// In order to add a cubical simplex to the complex, all elements
    /// of its boundary must already be part of the complex.
    /// 0 dimensional simplices can always be added.
    ///
    /// \param simplex Simplex to add
    ///
    /// \return true, if simplex has been successfully added, false
    ///         otherwise
    bool add(CubicalSimplex simplex);

    /// \brief Adds a cubical simplex to the complex together with
    ///        its boundary
    /// \param simplex Simplex to add
    void add_recursive(CubicalSimplex simplex);

    /// \brief Removes a cubical simplex from the complex
    ///
    /// In order to remove a cubical complex, it has to have an empty
    /// coboundary, that is, it cannot be a part of the boundary of any
    /// other simplex
    ///
    /// \param simplex Simplex to remove
    ///
    /// \return true, if removal was successfull, false otherwise
    bool remove(CubicalSimplex const& simplex);

    /// \brief Checks, if a cubical complex contains simplex
    ///
    /// \param simplex simplex to test
    ///
    /// \return true, if simplex is a part of the complex, false
    ///         otherwise
    bool contains(CubicalSimplex const& simplex) const;

    /// \brief Equality comparison operator
    bool operator==(CubicalComplex const&) const;

    /// \brief Grants access to the simplexes of the complex
    ///
    /// The simplices are returned in a vector, where `n`'th element
    /// is a unordered set containing simplices of dimension `n`.
    std::vector<std::unordered_set<CubicalSimplex>> const& simplices() const;

    /// \brief Returns the dimension of the simplex
    std::size_t dimension() const;

    /// \brief Returns the ambient dimension of the simplex
    std::size_t ambient_dimension() const;

private:
    void add_recursive_impl(CubicalSimplex simplex);

    std::vector<std::unordered_set<CubicalSimplex>> m_simplices;
};

} // namespace complexes

/// \brief std::hash specialization for Interval
template<>
struct std::hash<complexes::BasicInterval> {
    /// \brief Returns a hash of an interval
    std::size_t operator()(complexes::BasicInterval const& i) const;
};

/// \brief std::hash specialization for CubicalSimplex
template<>
struct std::hash<complexes::CubicalSimplex> {
    /// \brief Returns a hash of a cubical simplex
    std::size_t operator()(complexes::CubicalSimplex const& s) const;
};

/// \brief Formatter for BasicInterval type
///
/// Allows use of `std::format` with the `BasicInterval` type. The format
/// syntax is the same, as in the case of `int`.
template<>
struct std::formatter<complexes::BasicInterval>: public std::formatter<int> {
    /// \brief Formats an integer
    ///
    /// Formats an interval using a formatting syntax for int.
    template<class FmtContext>
    FmtContext::iterator
    format(complexes::BasicInterval i, FmtContext& ctx) const {
        std::format_to(ctx.out(), "[");
        std::formatter<int>::format(i.left(), ctx);
        if (!i.is_trivial()) {
            std::format_to(ctx.out(), ", ");
            std::formatter<int>::format(i.right(), ctx);
        }
        std::format_to(ctx.out(), "]");
        return ctx.out();
    }
};

/// \brief Formatter for CubicalSimplex type
///
/// Allows use of `std::format` with the `CubicalSimplex` type. The format
/// syntax is the same, as in the case of `BasicInterval`.
template<>
struct std::formatter<complexes::CubicalSimplex>:
    public std::formatter<complexes::BasicInterval> {
    /// \brief Formats a cubical simplex
    ///
    /// Formats an integer using a formatting syntax for an interval.
    template<class FmtContext>
    FmtContext::iterator
    format(complexes::CubicalSimplex const& s, FmtContext& ctx) const {
        namespace vs = std::views;
        std::formatter<complexes::BasicInterval>::format(
            s.intervals().front(),
            ctx
        );
        for (auto i : s.intervals() | vs::drop(1)) {
            std::format_to(ctx.out(), "x");
            std::formatter<complexes::BasicInterval>::format(i, ctx);
        }
        return ctx.out();
    }
};
