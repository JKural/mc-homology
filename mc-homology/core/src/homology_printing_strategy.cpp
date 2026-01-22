#include "../include/core/homology_printing_strategy.h"

#include <format>
#include <iterator>
#include <ranges>

#include "core/homology.h"

namespace core {

HomologyPrintingStrategy::~HomologyPrintingStrategy() = default;

std::string HomologyRawPrint::draw(Homology const& homology) const {
    return std::format("{}, {}", homology.betti_numbers(), homology.torsion());
}

HomologyLatexPrint::HomologyLatexPrint(
    std::string ring_name,
    std::string homology_name,
    std::string torsion_name
) :
    m_ring_name(std::move(ring_name)),
    m_homology_name(std::move(homology_name)),
    m_torsion_name(std::move(torsion_name)) {}

std::string HomologyLatexPrint::draw(Homology const& homology) const {
    namespace vs = std::views;
    std::string output;
    auto out = std::back_inserter(output);
    std::format_to(out, "\\begin{{align*}}");
    if (homology.betti_numbers().empty()) {
        std::format_to(out, "{} = 0", m_homology_name);
    } else {
        for (auto const& [i, betti_number, torsion] : vs::zip(
                 vs::iota(0),
                 homology.betti_numbers(),
                 homology.torsion()
             )) {
            std::format_to(
                out,
                "{}_{{{}}} &= {}^{{{}}}",
                m_homology_name,
                i,
                m_ring_name,
                betti_number
            );
            for (auto const& tor : torsion) {
                std::format_to(
                    out,
                    " \\oplus {}/{}{}",
                    m_ring_name,
                    tor,
                    m_ring_name
                );
            }
        }
    }
    std::format_to(out, "\\end{{align*}}");
    return output;
}

} // namespace core
