/// \file options.h
/// \brief File containing an Options class for storing user options
#pragma once

#include <filesystem>

namespace core {

/// \brief Enum for storing user's choice of homology to compute
enum class HomologyChoice {
    Z,
    Z2,
    Z3,
};

/// \bief A class for storing user options
class Options {
public:
    /// \brief Path to the region directory
    virtual std::filesystem::path filename() const = 0;

    /// \brief Bounds on x axis
    virtual std::pair<int, int> x_bounds() const = 0;

    /// \brief Bounds on y axis
    virtual std::pair<int, int> y_bounds() const = 0;

    /// \brief Bounds on y axis
    virtual std::pair<int, int> z_bounds() const = 0;

    /// \brief Type of homology to compute
    virtual HomologyChoice homology_to_compute() const = 0;

    /// \brief Whether to print latex output
    virtual bool latex() const = 0;

    /// \brief Whether the user requested help
    virtual bool help() const = 0;

    /// \brief virtual destructor
    virtual ~Options();
};

/// \brief A class for storing user's options from the
///        commandline
class CommandlineOptions: public Options {
public:
    /// \brief Parse commandline arguments
    CommandlineOptions(int argc, char** argv);

    /// \brief Path to the region directory
    std::filesystem::path filename() const override;

    /// \brief Bounds on x axis
    std::pair<int, int> x_bounds() const override;

    /// \brief Bounds on y axis
    std::pair<int, int> y_bounds() const override;

    /// \brief Bounds on y axis
    std::pair<int, int> z_bounds() const override;

    /// \brief Type of homology to compute
    HomologyChoice homology_to_compute() const override;

    /// \brief Whether to print latex output
    bool latex() const override;

    /// \brief Whether the user requested help
    bool help() const override;

private:
    std::filesystem::path m_filename = "";
    std::pair<int, int> m_x_bounds = {0, 0};
    std::pair<int, int> m_y_bounds = {0, 0};
    std::pair<int, int> m_z_bounds = {0, 0};
    HomologyChoice m_homology_to_compute = HomologyChoice::Z2;
    bool m_latex = false;
    bool m_help = false;
};

} // namespace core
