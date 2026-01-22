/// \file parser.h
/// \brief A file containing a minecraft savefile parser
#pragma once

#include <filesystem>

#include "complex.h"
#include "polymorphic.h"

namespace core {

/// \brief A struct containing coordinates in a Minecraft world
///
/// Minecraft world coordinates. Note, that y defines height.
struct MinecraftCoordinates {
    /// \brief x coordinate
    int x = 0;
    /// \brief y coordinate
    int y = 0;
    /// \brief z coordinate
    int z = 0;
};

/// \brief An interface for Minecraft savefile parser
class MinecraftSavefileParser {
public:
    /// \brief Parses a Minecraft savefile
    ///
    /// Parses a Minecraft savefile within given bounds.
    /// lower_corner.x <= upper_corner.x
    /// lower_corner.y <= upper_corner.y
    /// lower_corner.z <= upper_corner.z
    ///
    /// \param path Path to the save file region directory
    /// \paraw lower_corner Lower bounds on the studied cube
    /// \paraw upper_corner Upper bounds on the studied cube
    virtual Polymorphic<Complex> parse(
        std::filesystem::path const& path,
        MinecraftCoordinates lower_corner,
        MinecraftCoordinates upper_corner
    ) = 0;
    virtual ~MinecraftSavefileParser();
};

/// \brief A Minecraft savefile parser based on
///        https://github.com/TCA166/mcSavefileParsers.git
class MinecraftSavefileParser_mcSavefileParsers:
    public MinecraftSavefileParser {
    /// \brief Parses a Minecraft savefile
    ///
    /// Parses a Minecraft savefile within given bounds.
    /// lower_corner.x <= upper_corner.x
    /// lower_corner.y <= upper_corner.y
    /// lower_corner.z <= upper_corner.z
    ///
    /// \param path Path to the save file region directory
    /// \paraw lower_corner Lower bounds on the studied cube
    /// \paraw upper_corner Upper bounds on the studied cube
    Polymorphic<Complex> parse(
        std::filesystem::path const& path,
        MinecraftCoordinates lower_corner,
        MinecraftCoordinates upper_corner
    ) override;
};

} // namespace core
