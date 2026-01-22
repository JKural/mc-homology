#include "../include/core/homology.h"

#include <stdexcept>

namespace core {

Homology::~Homology() = default;

Homology::Homology(
    std::unique_ptr<HomologyPrintingStrategy> printing_strategy
) :
    m_printing_strategy(std::move(printing_strategy)) {
    if (!m_printing_strategy) {
        throw std::invalid_argument(
            "Printing strategy can't be a null pointer"
        );
    }
}

void Homology::select_strategy(
    std::unique_ptr<HomologyPrintingStrategy> printing_strategy
) {
    if (!printing_strategy) {
        throw std::invalid_argument(
            "Printing strategy can't be a null pointer"
        );
    }
    m_printing_strategy = std::move(printing_strategy);
}

std::string Homology::text() const {
    return m_printing_strategy->draw(*this);
}

} // namespace core
