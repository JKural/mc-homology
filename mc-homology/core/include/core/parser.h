/// \file parser.h
/// \brief A file containing a minecraft savefile parser
#pragma once

#include <filesystem>

#include "complex.h"
#include "polymorphic.h"

namespace core {

/// \brief An interface for Minecraft savefile parser
class MinecraftSavefileParser {
public:
    virtual Polymorphic<Complex> parse(std::filesystem::path const& path) = 0;
    virtual ~MinecraftSavefileParser();
};

/// \brief A Minecraft savefile parser based on
///        https://github.com/TCA166/mcSavefileParsers.git
class MinecraftSavefileParser_mcSavefileParsers:
    public MinecraftSavefileParser {
    Polymorphic<Complex> parse(std::filesystem::path const& path) override;
};

} // namespace core
