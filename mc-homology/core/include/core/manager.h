/// \file manager.h
/// \brief A file containing main logic of the program
#pragma once

#include <memory>

#include "core/options.h"
#include "core/parser.h"

namespace core {

/// \brief Main manager for the program
class Manager {
public:
    /// \brief Construct a new manager with selected parser and options
    Manager(
        std::unique_ptr<Options> options,
        std::unique_ptr<MinecraftSavefileParser> parser
    );

    /// \brief Select options
    void set_options(std::unique_ptr<Options> options);

    /// \brief Select parser
    void set_parser(std::unique_ptr<MinecraftSavefileParser> parser);

    /// \brief Start the program
    ///
    /// \return program return status
    int run();

private:
    std::unique_ptr<Options> m_options;
    std::unique_ptr<MinecraftSavefileParser> m_parser;
};

} // namespace core
