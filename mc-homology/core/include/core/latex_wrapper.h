/// \file latex_wrapper.h
/// \brief Turns a TextDrawable into a simple latex document
#pragma once

#include <memory>

#include "core/text_drawable.h"

namespace core {

/// \brief A decorator that wraps a text drawable object in a latex
///        document
class LatexWrapper: public TextDrawable {
public:
    /// \brief Constructs the decorator with the object to wrap
    LatexWrapper(
        std::unique_ptr<TextDrawable> inner,
        std::string documentclass = "article"
    );

    /// \brief Returns the text representation of the object
    std::string text() const override;

private:
    /// \brief Wrapped (decorated) value
    std::unique_ptr<TextDrawable> m_inner;
    /// \brief Document class of the latex file
    std::string m_documentclass;
};

} // namespace core
