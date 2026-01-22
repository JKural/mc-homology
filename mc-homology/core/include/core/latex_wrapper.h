/// \file latex_wrapper.h
/// \brief Turns a TextDrawable into a simple latex document
#pragma once

#include "polymorphic.h"
#include "text_drawable.h"

namespace core {

/// \brief A decorator that wraps a text drawable object in a latex
///        document
class LatexWrapper: public TextDrawable {
public:
    /// \brief Constructs the decorator with the object to wrap
    LatexWrapper(Polymorphic<TextDrawable> inner);

    /// \brief Returns the text representation of the object
    std::string text() const override;

private:
    Polymorphic<TextDrawable> m_inner;
};

} // namespace core
