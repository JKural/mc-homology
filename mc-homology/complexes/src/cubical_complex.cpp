#include "../include/complexes/cubical_complex.h"

#include <algorithm>
#include <cassert>
#include <compare>
#include <stdexcept>
#include <unordered_set>
#include <vector>

#include "../include/complexes/utils.h"

namespace complexes {

BasicInterval::BasicInterval() = default;

std::size_t BasicInterval::hash() const {
    return utils::combine_hashes(
        std::hash<int> {}(m_left),
        std::hash<bool> {}(m_full)
    );
}

bool BasicInterval::operator==(BasicInterval const&) const = default;

int BasicInterval::left() const {
    return m_left;
}

int BasicInterval::right() const {
    if (m_full) {
        return m_left + 1;
    } else {
        return m_left;
    }
}

bool BasicInterval::is_trivial() const {
    return !m_full;
}

BasicInterval BasicInterval::point(int p) {
    BasicInterval i {};
    i.m_left = p;
    return i;
}

BasicInterval BasicInterval::interval(int left) {
    BasicInterval i {};
    i.m_left = left;
    i.m_full = true;
    return i;
}

std::ostream& operator<<(std::ostream& output, BasicInterval i) {
    return output << std::format("{}", i);
}

CubicalSimplex::CubicalSimplex(std::vector<BasicInterval> intervals) :
    m_intervals(std::move(intervals)),
    m_dimension(std::ranges::count_if(m_intervals, [](BasicInterval const& i) {
        return !i.is_trivial();
    })) {
    if (m_intervals.empty()) {
        throw std::domain_error("Intervals cannot be empty");
    }
}

std::size_t CubicalSimplex::dimension() const {
    return m_dimension;
}

std::size_t CubicalSimplex::ambient_dimension() const {
    return m_intervals.size();
}

std::size_t CubicalSimplex::hash() const {
    return utils::hash_range(m_intervals);
}

std::vector<CubicalSimplex> CubicalSimplex::boundary() const {
    namespace vs = std::views;
    std::vector<CubicalSimplex> boundary {};
    for (auto const& [n, i] : m_intervals | vs::enumerate) {
        if (!i.is_trivial()) {
            auto top_boundary = m_intervals;
            top_boundary[n] = BasicInterval::point(i.right());
            auto bottom_boundary = m_intervals;
            bottom_boundary[n] = BasicInterval::point(i.left());
            boundary.emplace_back(std::move(top_boundary));
            boundary.emplace_back(std::move(bottom_boundary));
        }
    }
    return boundary;
}

bool CubicalSimplex::operator==(CubicalSimplex const&) const = default;

std::vector<BasicInterval> const& CubicalSimplex::intervals() const {
    return m_intervals;
}

std::strong_ordering
CubicalSimplex::operator<=>(CubicalSimplex const& simplex) const {
    auto dim_ordering = dimension() <=> simplex.dimension();
    if (dim_ordering != std::strong_ordering::equal) {
        return dim_ordering;
    }
    return std::lexicographical_compare_three_way(
        m_intervals.begin(),
        m_intervals.end(),
        simplex.m_intervals.begin(),
        simplex.m_intervals.end(),
        [](BasicInterval i1, BasicInterval i2) {
            if (!i1.is_trivial() && i2.is_trivial()) {
                return std::strong_ordering::less;
            } else if (i1.is_trivial() && !i2.is_trivial()) {
                return std::strong_ordering::greater;
            } else {
                return i1.left() <=> i2.left();
            }
        }
    );
}

CubicalSimplex CubicalSimplex::point(int p) {
    return CubicalSimplex(std::vector {BasicInterval::point(p)});
}

CubicalSimplex CubicalSimplex::interval(int left) {
    return CubicalSimplex(std::vector {BasicInterval::interval(left)});
}

CubicalSimplex::CubicalSimplex() = default;

CubicalSimplex product(CubicalSimplex const& s1, CubicalSimplex const& s2) {
    CubicalSimplex result {};
    result.m_intervals = s1.m_intervals;
    result.m_intervals.append_range(s2.m_intervals);
    result.m_dimension = s1.m_dimension + s2.m_dimension;
    return result;
}

std::ostream& operator<<(std::ostream& output, CubicalSimplex const& s) {
    return output << std::format("{}", s);
}

CubicalComplex::CubicalComplex() = default;

bool CubicalComplex::add(CubicalSimplex simplex) {
    if (ambient_dimension() != 0
        && ambient_dimension() != simplex.ambient_dimension()) {
        throw std::domain_error(
            "All simplices of a complex must be of the same ambient dimension"
        );
    }
    auto dimension = simplex.dimension();
    if (dimension == 0) {
        if (m_simplices.empty()) {
            m_simplices.emplace_back(
                std::unordered_set<CubicalSimplex> {std::move(simplex)}
            );
            return true;
        } else {
            auto [_, success] = m_simplices[0].emplace(std::move(simplex));
            return success;
        }
    } else if (dimension > this->dimension() + 1) {
        return false;
    } else {
        if (std::ranges::all_of(
                simplex.boundary(),
                [this](CubicalSimplex const& boundary_simplex) {
                    return this->contains(boundary_simplex);
                }
            )) {
            if (dimension == this->dimension() + 1) {
                m_simplices.emplace_back(
                    std::unordered_set<CubicalSimplex> {std::move(simplex)}
                );
                return true;
            } else {
                auto [_, success] =
                    m_simplices[dimension].emplace(std::move(simplex));
                return success;
            }
        } else {
            return false;
        }
    }
}

void CubicalComplex::add_recursive(CubicalSimplex simplex) {
    if (ambient_dimension() != 0
        && ambient_dimension() != simplex.ambient_dimension()) {
        throw std::domain_error(
            "All simplices of a complex must be of the same ambient dimension"
        );
    }
    m_simplices.resize(
        std::ranges::max(simplex.dimension() + 1, dimension() + 1)
    );
    add_recursive_impl(std::move(simplex));
}

bool CubicalComplex::remove(CubicalSimplex const& simplex) {
    if (simplex.dimension() > dimension()) {
        return false;
    } else if (simplex.dimension() == dimension()) {
        m_simplices[simplex.dimension()].erase(simplex);
        if (m_simplices[simplex.dimension()].empty()) {
            m_simplices.pop_back();
        }
        return true;
    } else {
        for (auto const& higher_dimensional_simplices :
             m_simplices[simplex.dimension() + 1]) {
            if (std::ranges::binary_search(
                    higher_dimensional_simplices.boundary(),
                    simplex
                )) {
                return false;
            }
        }
        m_simplices[simplex.dimension()].erase(simplex);
        return true;
    }
}

bool CubicalComplex::contains(CubicalSimplex const& simplex) const {
    return simplex.dimension() <= dimension()
        ? m_simplices[simplex.dimension()].contains(simplex)
        : false;
}

bool CubicalComplex::operator==(CubicalComplex const&) const = default;

std::vector<std::unordered_set<CubicalSimplex>> const&
CubicalComplex::simplices() const {
    return m_simplices;
}

std::size_t CubicalComplex::dimension() const {
    if (m_simplices.empty()) {
        return 0;
    }
    return m_simplices.size() - 1;
}

std::size_t CubicalComplex::ambient_dimension() const {
    if (m_simplices.empty()) {
        return 0;
    }
    assert(!m_simplices[0].empty());
    return m_simplices[0].begin()->ambient_dimension();
}

void CubicalComplex::add_recursive_impl(CubicalSimplex simplex) {
    auto const dim = simplex.dimension();
    auto [it, success] = m_simplices[dim].emplace(std::move(simplex));
    if (success && dim != 0) {
        for (auto simplex : it->boundary()) {
            add_recursive_impl(std::move(simplex));
        }
    }
}

} // namespace complexes

std::size_t std::hash<complexes::BasicInterval>::operator()(
    complexes::BasicInterval const& i
) const {
    return i.hash();
}

std::size_t std::hash<complexes::CubicalSimplex>::operator()(
    complexes::CubicalSimplex const& s
) const {
    return s.hash();
}
