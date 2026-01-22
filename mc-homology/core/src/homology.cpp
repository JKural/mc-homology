#include "../include/core/homology.h"

namespace core {

Homology::~Homology() = default;

Homology::Homology(Polymorphic<HomologyPrintingStrategy> printing_strategy) :
    m_printing_strategy(std::move(printing_strategy)) {}

void Homology::select_strategy(
    Polymorphic<HomologyPrintingStrategy> printing_strategy
) {
    m_printing_strategy = std::move(printing_strategy);
}

std::string Homology::text() const {
    return m_printing_strategy->draw(*this);
}

} // namespace core
