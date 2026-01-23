#include "../include/core/options.h"

#include <cstring>
#include <stdexcept>

namespace core {

Options::~Options() = default;

CommandlineOptions::CommandlineOptions(int argc, char** argv) {
    if (argc < 2) {
        throw std::invalid_argument("At least 2 arguments required");
    }
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "-h") == 0
            || std::strcmp(argv[i], "--help") == 0) {
            m_help = true;
            return;
        }
    }
    for (int i = 1; i < argc; ++i) {
        if (std::strncmp(argv[i], "-", 1)) {
            if (m_filename.empty()) {
                m_filename = std::string(argv[i]);
            } else {
                throw std::invalid_argument("Two filenames");
            }
        } else if (std::strcmp(argv[i], "--Z2") == 0) {
            m_homology_to_compute = HomologyChoice::Z2;
        } else if (std::strcmp(argv[i], "--Z3") == 0) {
            m_homology_to_compute = HomologyChoice::Z3;
        } else if (std::strcmp(argv[i], "--Z") == 0) {
            m_homology_to_compute = HomologyChoice::Z;
        } else if (std::strcmp(argv[i], "--latex") == 0) {
            m_latex = true;
        } else if (std::strcmp(argv[i], "--no-latex") == 0) {
            m_latex = false;
        } else if (std::strcmp(argv[i], "--x")) {
            if (i + 2 >= argc) {
                throw std::invalid_argument("Not enough arguments for --x");
            }
            try {
                m_x_bounds = {std::stoi(argv[i + 1]), std::stoi(argv[i + 2])};
            } catch (std::invalid_argument&) {
                throw std::invalid_argument("Expected numbers for bounds");
            }
            i += 2;
        } else if (std::strcmp(argv[i], "--y")) {
            if (i + 2 >= argc) {
                throw std::invalid_argument("Not enough arguments for --y");
            }
            try {
                m_y_bounds = {std::stoi(argv[i + 1]), std::stoi(argv[i + 2])};
            } catch (std::invalid_argument&) {
                throw std::invalid_argument("Expected numbers for bounds");
            }
            i += 2;
        } else if (std::strcmp(argv[i], "--z")) {
            if (i + 2 >= argc) {
                throw std::invalid_argument("Not enough arguments for --z");
            }
            try {
                m_z_bounds = {std::stoi(argv[i + 1]), std::stoi(argv[i + 2])};
            } catch (std::invalid_argument&) {
                throw std::invalid_argument("Expected numbers for bounds");
            }
            i += 2;
        }
    }
    if (m_filename.empty()) {
        throw std::invalid_argument("Filename not specified");
    }
}

std::filesystem::path CommandlineOptions::filename() const {
    return m_filename;
}

std::pair<int, int> CommandlineOptions::x_bounds() const {
    return m_x_bounds;
}

std::pair<int, int> CommandlineOptions::y_bounds() const {
    return m_y_bounds;
}

std::pair<int, int> CommandlineOptions::z_bounds() const {
    return m_z_bounds;
}

HomologyChoice CommandlineOptions::homology_to_compute() const {
    return m_homology_to_compute;
}

bool CommandlineOptions::latex() const {
    return m_latex;
}

bool CommandlineOptions::help() const {
    return m_help;
}

} // namespace core
