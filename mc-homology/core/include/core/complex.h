/// \file complex.h
/// \brief A file containing interface for the Complex class
#pragma once

#include "homology.h"

namespace core {

/// \brief Interface for complexes
class Complex {
public:
    /// \brief Computes Z2 homology of the complex
    virtual Polymorphic<Homology> z2_homology() const = 0;

    /// \brief Computes Z3 homology of the complex
    virtual Polymorphic<Homology> z3_homology() const = 0;

    /// \brief Computes Z homology of the complex
    virtual Polymorphic<Homology> z_homology() const = 0;

    /// \brief Decreases the complex's size without changing its
    ///        homology
    virtual void reduce() = 0;

    /// \brief Virtual destructor
    virtual ~Complex();
};

} // namespace core
