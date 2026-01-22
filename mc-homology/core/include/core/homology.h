/// \file homology.h
/// \brief A file containing a Homology interface
#pragma once

#include <memory>
#include <vector>

#include "core/homology_printing_strategy.h"
#include "core/text_drawable.h"

namespace core {

/// \brief A homology interface
///
/// Creates a common interface for various homology implementations.
/// Additionally handles printing the result.
class Homology: public TextDrawable {
public:
    /// \brief Constructs a new homology instance
    ///
    /// Constructs a new homology object with a specified printing
    /// strategy
    ///
    /// \param printing_strategy A strategy used with printing
    Homology(
        std::unique_ptr<HomologyPrintingStrategy> printing_strategy =
            std::make_unique<HomologyRawPrint>()
    );

    /// \brief Select a new printing_strategy
    ///
    /// \param printing_strategy A strategy used with printing
    void select_strategy(
        std::unique_ptr<HomologyPrintingStrategy> printing_strategy
    );

    /// \brief Outputs the test form of the homology using given
    ///        strategy
    std::string text() const override;

    /// \brief Returns text representations of stored betti numbers
    virtual std::vector<std::string> betti_numbers() const = 0;

    /// \brief Returns text representations of stored torsion as the
    ///        non-trivial part of the Smith's form diagonal
    virtual std::vector<std::vector<std::string>> torsion() const = 0;

    // \brief Virtual destructor
    virtual ~Homology();

private:
    std::unique_ptr<HomologyPrintingStrategy> m_printing_strategy;
};

} // namespace core
