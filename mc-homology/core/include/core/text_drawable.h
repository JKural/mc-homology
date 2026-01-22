/// \file text_drawable.h
/// \brief A file containing a TextDrawable interface, for classes that
///        can be drawn on screen in the form of text.
#pragma once

#include <string>

namespace core {

/// \brief Marks a class text drawable - it can be represented as a
///        string
class TextDrawable {
public:
    /// \brief Returns the text representation of the object
    virtual std::string text() const = 0;

    /// \brief Virtual destructor
    virtual ~TextDrawable();
};

} // namespace core
