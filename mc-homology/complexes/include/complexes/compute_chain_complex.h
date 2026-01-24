/// \file compute_chain_complex.h
/// \brief A file containing algorithms for transforming complexes into
///        chain complexes.
#pragma once

#include <ranges>
#include <unordered_map>
#include <vector>

#include "algebra/chain_complex.h"
#include "algebra/matrix.h"
#include "cubical_complex.h"

namespace complexes {

/// \brief Computes a chain complex from a cubical complex
///
/// Transforms relationships between faces into boundary operators
///
/// \param cubical_complex Complex to transorm
///
/// \result A chain complex
template<class T>
algebra::ChainComplex<T>
compute_chain_complex(CubicalComplex const& cubical_complex) {
    namespace vs = std::views;
    auto const& simplices = cubical_complex.simplices();
    if (simplices.empty()) {
        return algebra::ChainComplex<T> {};
    }
    std::vector<algebra::Matrix<T>> boundaries(simplices.size());
    // 0'th dimensional matrix is empty, we are computing non-reduced
    // homology
    boundaries[0] = algebra::Matrix<T>::zero(0, simplices[0].size());
    for (std::size_t dim = 1; dim < simplices.size(); ++dim) {
        boundaries[dim] = algebra::Matrix<T>::zero(
            simplices[dim - 1].size(),
            simplices[dim].size()
        );
        std::unordered_map<CubicalSimplex, std::size_t> assigned_rows;
        for (auto const& [s, i] : vs::zip(simplices[dim - 1], vs::iota(0))) {
            assigned_rows.emplace(s, i);
        }
        for (auto const& [j, simplex] : simplices[dim] | vs::enumerate) {
            std::array sgn = {1, -1, -1, 1};
            for (auto const& [k, bd] : simplex.boundary() | vs::enumerate) {
                boundaries[dim][assigned_rows[bd], j] = sgn[k % sgn.size()];
            }
        }
    }
    return algebra::ChainComplex<T> {std::move(boundaries)};
}

} // namespace complexes
