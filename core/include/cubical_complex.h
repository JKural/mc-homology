/// \file cubical_complex.h
/// \brief A file containing the CubicalComplex class

#pragma once

#include <complex.h>

#include <unordered_map>
#include <vector>

#include "complex.h"

namespace core {

/// \brief A struct representing an interval of length 0 or 1
///
/// A struct representing an interval of a form [`left`, `left`] or
/// [`left`, `left + 1`]
struct Interval {
    /// \brief Computer a hash of the inverval
    std::size_t hash() const {
        return utils::combine_hashes(
            std::hash<std::size_t> {}(left),
            std::hash<bool> {}(full)
        );
    }

    /// \brief Compares two intervals
    bool operator==(Interval const&) const = default;

    /// \brief Left end of the interval
    int left = 0;

    /// \brief Marker, if the interval is of length 1 or 0
    bool full = false;
};

/// \brief A class representing a single cubical simplex in a complex
class CubicalSimplex {
public:
    /// \brief Returns the dimension of the simplex
    std::size_t dimension() const;

    /// \brief Computes a hash of the simplex
    std::size_t hash() const;

    /// \brief Compares two simplices
    bool operator==(CubicalSimplex const&) const = default;

private:
    std::vector<Interval> m_intervals;
};

class CubicalComplex: public IComplex {
public:
    /// \brief Default constructor for the CubicalComplex
    ///
    /// Creates an empty complex
    CubicalComplex() = default;

    std::size_t dimension() const override;

    void accept(IHomologyVisitor&) const override;

    std::string to_string() const override;

    /// \brief Adds a cubical simplex to the complex
    ///
    /// In order to add a cubical simplex to the complex, all elements
    /// of its boundary must already be part of the complex.
    /// 0 dimensional simplices can always be added.
    void add(CubicalSimplex simplex);

private:
};

} // namespace core
