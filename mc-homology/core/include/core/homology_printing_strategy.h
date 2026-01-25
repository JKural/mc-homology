/// \file homology_printing_strategy.h
/// \brief File containing strategies for printing homology
#pragma once

#include <string>

namespace core {

class Homology;

/// \brief A strategy for printing a homology group
class HomologyPrintingStrategy {
public:
    /// \brief Returns the text representation of the homology group
    virtual std::string draw(Homology const& homology) const = 0;

    /// \brief A virtual destructor
    virtual ~HomologyPrintingStrategy();
};

/// \brief A strategy for basic string representation of homology
class HomologyRawPrint: public HomologyPrintingStrategy {
public:
    /// \brief Prints betti numbers and torsion group
    std::string draw(Homology const& homology) const override;
};

/// \brief Prints a homology group in Latex syntax
///
/// Prints a homology group in Latex syntax, making it easily
/// embeddable into a Latex file.
class HomologyLatexPrint: public HomologyPrintingStrategy {
public:
    /// \brief Constructor for the strategy
    ///
    /// Constructor for the strategy, allows to select names for the
    /// homology and torsion groups
    ///
    /// \param ring_name Name for the coefficient ring
    /// \param homology_name Name for the homology group
    HomologyLatexPrint(std::string ring_name, std::string homology_name = "H");

    /// \brief Returns the homology description in latex syntax
    std::string draw(Homology const& homology) const override;

private:
    std::string m_ring_name;
    std::string m_homology_name;
};

} // namespace core
