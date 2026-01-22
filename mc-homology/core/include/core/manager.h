/// \file mediator.h
/// \brief A file containing main logic of the program
#pragma once

#include <memory>

#include "core/options.h"

namespace core {

/// \brief Main manager for the program
class Manager {
public:
    /// \brief Construct from options
    Manager(std::unique_ptr<Options> options);

    /// \brief Select options
    void set_options(std::unique_ptr<Options> options);

    /// \brief Start the program
    ///
    /// \return program return status
    int run();

private:
    std::unique_ptr<Options> m_options;
};

} // namespace core
