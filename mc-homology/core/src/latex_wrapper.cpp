#include "../include/core/latex_wrapper.h"

#include <sstream>
#include <stdexcept>

namespace core {

LatexWrapper::LatexWrapper(
    std::unique_ptr<TextDrawable> inner,
    std::string documentclass
) :
    m_inner(std::move(inner)),
    m_documentclass(std::move(documentclass)) {
    if (!m_inner) {
        throw std::invalid_argument("Inner data can't be a null pointer");
    }
}

std::string LatexWrapper::text() const {
    std::stringstream output;
    output << R"(\documentclass{)" << m_documentclass << R"(}
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
