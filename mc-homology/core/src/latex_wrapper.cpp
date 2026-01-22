#include "../include/core/latex_wrapper.h"

#include <sstream>

#include "core/polymorphic.h"

namespace core {

LatexWrapper::LatexWrapper(Polymorphic<TextDrawable> inner) :
    m_inner(std::move(inner)) {}

std::string LatexWrapper::text() const {
    std::stringstream output;
    output << R"(\documentclass{article}
\usepackage[T1]{fontenc}
\usepackage[english]{babel}
\usepackage{amsmath}
\usepackage{amsfonts}

\begin{document}

)";
    output << m_inner->text();
    output << R"(

\end{document}
)";
    return output.str();
}

} // namespace core
