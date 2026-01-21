/// \file homology.h
/// \brief A file containing an interface for homology visitor.

#include "complex.h"

namespace core {

/// \brief Homology visitor for the Complex class
///
/// Computes homology of a complex
class IHomologyVisitor: public utils::IStringPrintable {
public:
    /// \brief Computes homology of a complex
    void visit(IComplex const&);
};

} // namespace core
