/// \file chain_complex.h
/// \brief A file containing chain complex and homology implementations

#pragma once

#include "algebra/matrix_algorithms.h"

namespace algebra {

/// \brief Type to mark an overload of a function, that doesn't
///        perform optional correctness checks. Such overloads
///        should only be used after manually ensuring, that
///        other parameters guarantee correct output.
///
///        Use with caution!
struct SkipCorrectnessCheckT {};

/// \brief Helper value of that SkipCorrectnessCheckT
constexpr inline SkipCorrectnessCheckT skip_correctness_check {};

/// \brief Class representing a chain complex with coefficients `T`
///
/// Chain complex is a free module over T with submodules
/// `(C_d, ..., C_2, C_1, C_0, C_(-1))` and linear operators
/// `B_n : C_n -> C_(n-1)` satisfying B_n*B_(n+1) == 0
/// (the chain complex condidition).
///
/// \tparam T Class of the coefficients
template<class T>
class ChainComplex {
public:
    constexpr ChainComplex() = default;

    /// \brief Constructs the chain complex from boundary matrices and
    /// checks, if they satisfy the chain complex condition.
    ///
    /// \param boundaries The range containing the boundary opeartors
    template<std::ranges::sized_range R>
        requires std::convertible_to<std::ranges::range_value_t<R>, Matrix<T>>
    constexpr ChainComplex(R&& boundaries) :
        ChainComplex(skip_correctness_check, std::forward<R>(boundaries)) {
        if (!check_boundary_correctness()) {
            throw std::domain_error(
                "The boundary matrices do not satisfy chain complex condition"
            );
        }
    }

    /// \brief Constructs the chain complex from the boundary matrices
    ///        without checking the chain complex condition
    ///
    /// Use with caution!
    ///
    /// \param boundaries The range containing the boundary opeartors
    template<std::ranges::sized_range R>
        requires std::convertible_to<std::ranges::range_value_t<R>, Matrix<T>>
    constexpr ChainComplex(SkipCorrectnessCheckT, R&& boundaries) :
        m_boundaries(
            std::ranges::to<std::vector<Matrix<T>>>(std::forward<R>(boundaries))
        ) {}

    /// \brief Checks, if the boundaries satisfy the chain complex
    ///        condition.
    constexpr bool check_boundary_correctness() const {
        if (m_boundaries.size() < 2) {
            return true;
        }
        namespace rs = std::ranges;
        namespace vs = std::views;
        auto b_n_plus_1_view = m_boundaries | vs::drop(1);
        auto b_n_view = m_boundaries | vs::take(m_boundaries.size() - 1);
        return rs::all_of(
            vs::zip(b_n_plus_1_view, b_n_view),
            [](auto const& boundaries) {
                try {
                    auto const& [b_n_plus_1, b_n] = boundaries;
                    return (b_n * b_n_plus_1).is_zero();
                } catch (std::domain_error&) {
                    return false;
                }
            }
        );
    }

    /// \brief Return the dimension (the number of boundary matrices
    /// minus 1) of the chain complex
    constexpr std::size_t dimension() const noexcept {
        return m_boundaries.size();
    }

    /// \brief Return the boundary operator at dimension `dim`
    ///
    /// \param dim Dimension of the boundary operator
    constexpr Matrix<T> const& boundary(std::size_t dim) const {
        return m_boundaries.at(dim);
    }

    /// \brief Return the vector of boundary operators
    constexpr std::vector<Matrix<T>> const& boundaries() const noexcept {
        return m_boundaries;
    }

private:
    /// \brief A vector of boundary operators
    std::vector<Matrix<T>> m_boundaries;
};

/// \brief Homology of a chain complex
///
/// A struct containing homology information of a chain complex - its
/// betti numbers and torsion group.
/// The data is stored increasingly in dimension:
/// dim(H_0) = betti_numbers[0], dim(H_1) = betti_numbers[1] itd.
template<class T>
struct Homology {
    /// \brief Betti numbers of a chain complex
    std::vector<std::size_t> betti_numbers {};

    /// \brief Torsion of a chain complex
    ///
    /// Information about the torsion group is stored in the following
    /// way: torsion[n] contains an array of elements such that
    /// torsion is equal to the simple sum of T/aT for a in the array.
    std::vector<std::vector<T>> torsion {};
};

/// \brief Computes homology of a chain complex with coefficients from
///        an euclidean domain
template<EuclideanDomain T>
Homology<T> homology(ChainComplex<T> const& chain_complex) {
    namespace rs = std::ranges;
    namespace vs = std::views;
    auto const& boundaries = chain_complex.boundaries();
    Homology<T> homology;
    homology.betti_numbers.resize(boundaries.size());
    homology.torsion.resize(boundaries.size());
    std::size_t prev_smith_units_count = 0;
    std::vector<T> prev_smith_diagonal_without_units {};
    for (std::size_t k = boundaries.size(); k > 0; --k) {
        auto const n = k - 1;
        auto const& boundary = boundaries[n];
        auto [smith, rank] = smith_form(boundary);
        auto nullity = boundary.ncols() - rank;
        auto smith_diagonal_without_units = rs::to<std::vector>(
            vs::iota(0u, rank)
            | vs::transform([&smith](std::size_t n) { return smith[n, n]; })
            | vs::drop_while([](T const& x) {
                  return x.euclidean_function() == 1;
              })
        );
        auto smith_units = rank - smith_diagonal_without_units.size();
        homology.betti_numbers[n] = nullity - prev_smith_units_count
            - prev_smith_diagonal_without_units.size();
        homology.torsion[n] = std::move(prev_smith_diagonal_without_units);
        prev_smith_units_count = smith_units;
        prev_smith_diagonal_without_units =
            std::move(smith_diagonal_without_units);
    }
    return homology;
}

/// \brief Computes homology of a chain complex with coefficients from
///        a field
template<Field T>
Homology<T> homology(ChainComplex<T> const& chain_complex) {
    auto const& boundaries = chain_complex.boundaries();
    Homology<T> homology;
    homology.betti_numbers.resize(boundaries.size());
    homology.torsion.resize(boundaries.size());
    std::size_t prev_rank = 0;
    for (std::size_t k = boundaries.size(); k > 0; --k) {
        auto const n = k - 1;
        auto const& boundary = boundaries[n];
        auto [_, rank] = row_echelon_form(boundary);
        auto nullity = boundary.ncols() - rank;
        homology.betti_numbers[n] = nullity - prev_rank;
        prev_rank = rank;
    }
    return homology;
}

/// \brief Deduction guide for the ChainComplex
template<std::ranges::sized_range R>
ChainComplex(SkipCorrectnessCheckT, R&&)
    -> ChainComplex<typename std::ranges::range_value_t<R>::value_type>;

/// \brief Deduction guide for the ChainComplex
template<std::ranges::sized_range R>
ChainComplex(R&&)
    -> ChainComplex<typename std::ranges::range_value_t<R>::value_type>;

} // namespace algebra
