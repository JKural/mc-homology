/// \file complex.h
/// \brief A file containing interface for a complex
///
/// A complex is a combinatorial representation of a topological
/// space. This representation allows us to compute certain
/// topological invariants of a given space, in particular
/// its homology.

#pragma once

#include "utils.h"

namespace core {

class IHomologyVisitor;

/// \brief Interface for a Complex
///
/// Complex is a combinatorial representation of a topological space,
/// which enables us to compute its topological invariants such as
/// homology.
class IComplex: public utils::IStringPrintable {
    /// \brief Returns the dimension of the complex
    ///
    /// Returns the dimension of the complex, that is the dimension
    /// of its highest dimensional cell(s).
    virtual std::size_t dimension() const = 0;

    /// \brief Accepts a Homology visitor
    ///
    /// Accepts a Homology visitor, enabling us to compute its
    /// homology.
    virtual void accept(IHomologyVisitor&) const = 0;
};

} // namespace core
