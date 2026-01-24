#include "../include/core/homology_printing_strategy.h"

#include <algorithm>
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
    std::string homology_name
) :
    m_ring_name(std::move(ring_name)),
    m_homology_name(std::move(homology_name)) {}

std::string HomologyLatexPrint::draw(Homology const& homology) const {
    namespace rs = std::ranges;
    namespace vs = std::views;
    std::string output;
    auto out = std::back_inserter(output);
    std::format_to(out, "\\begin{{align*}}\n");
    if (homology.betti_numbers().empty()
        && rs::all_of(
            vs::zip(homology.betti_numbers(), homology.torsion()),
            [](auto const& iv) {
                auto const& [i, v] = iv;
                return i == 0 && v.empty();
            }
        )) {
        std::format_to(out, "    {} &= 0\n", m_homology_name);
    } else {
        for (auto const& [i, betti_number, torsion] : vs::zip(
                 vs::iota(0),
                 homology.betti_numbers(),
                 homology.torsion()
             )) {
            switch (betti_number) {
                case 0: {
                    std::format_to(
                        out,
                        "    {}_{{{}}} &= 0\n",
                        m_homology_name,
                        i
                    );
                    break;
                }
                case 1: {
                    std::format_to(
                        out,
                        "    {}_{{{}}} &= {}\n",
                        m_homology_name,
                        i,
                        m_ring_name
                    );
                    break;
                }
                default: {
                    std::format_to(
                        out,
                        "    {}_{{{}}} &= {}^{{{}}}\n",
                        m_homology_name,
                        i,
                        m_ring_name,
                        betti_number
                    );
                    break;
                }
            }
        }
    }
    std::format_to(out, "\\end{{align*}}");
    return output;
}

} // namespace core
