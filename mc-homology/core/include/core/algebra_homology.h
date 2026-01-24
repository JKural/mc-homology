/// \file algebra_homology.h
/// \brief File containing concrete classes deriving from Homology
#pragma once

#include <memory>
#include <ranges>

#include "algebra/chain_complex.h"
#include "core/homology.h"
#include "core/homology_printing_strategy.h"

namespace core {

/// \brief Concrete subclass of Homology, based on algebra library
template<class T>
class AlgebraHomology: public Homology {
public:
    /// \brief Constructs homology and stores printing strategy
    AlgebraHomology(
        algebra::Homology<T> homology,
        std::unique_ptr<HomologyPrintingStrategy> printing_strategy =
            std::make_unique<HomologyRawPrint>()
    ) :
        Homology(std::move(printing_strategy)),
        m_homology(std::move(homology)) {}

    /// \brief Returns text representation of the betti numbers
    std::vector<std::size_t> betti_numbers() const override {
        return m_homology.betti_numbers;
    }

    /// \brief Returns text represetnation of torsion
    std::vector<std::vector<std::string>> torsion() const override {
        namespace rs = std::ranges;
        namespace vs = std::views;
        return rs::to<std::vector>(
            m_homology.torsion | vs::transform([](auto const& tor) {
                return rs::to<std::vector>(tor | vs::transform([](auto x) {
                                               return std::format("{}", x);
                                           }));
            })
        );
    }

private:
    algebra::Homology<T> m_homology;
};

} // namespace core
