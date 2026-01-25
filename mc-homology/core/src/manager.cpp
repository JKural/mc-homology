#include "../include/core/manager.h"

#include <print>
#include <stdexcept>
#include <utility>

#include "core/homology_printing_strategy.h"
#include "core/latex_wrapper.h"
#include "core/options.h"
#include "core/parser.h"
#include "core/text_drawable.h"

namespace core {

Manager::Manager(
    std::unique_ptr<Options> options,
    std::unique_ptr<MinecraftSavefileParser> parser
) :
    m_options(std::move(options)),
    m_parser(std::move(parser)) {
    if (!m_options) {
        throw std::invalid_argument("Options can't be null");
    }
    if (!m_parser) {
        throw std::invalid_argument("Parser can't be null");
    }
}

void Manager::set_options(std::unique_ptr<Options> options) {
    if (!options) {
        throw std::invalid_argument("Options can't be null");
    }
    m_options = std::move(options);
}

void Manager::set_parser(std::unique_ptr<MinecraftSavefileParser> parser) {
    if (!parser) {
        throw std::invalid_argument("Parser can't be null");
    }
    m_parser = std::move(parser);
}

int Manager::run() {
    if (m_options->help()) {
        std::println("Usage:");
        std::println(
            "mc-homology [-h | --help] [--Z | --Z2 | --Z3] [--latex | --no-latex] \\\n"
            "  [--x <x1> <x2>] [--y <y1> <y2>] [--z <z1> <z2>] <path-to-region-directory>"
        );
        std::println("Options:");
        std::println("-h | --help");
        std::println("  Print help and exit.");
        std::println("--Z | --Z2 | --Z3");
        std::println("  Choose coefficients of the chain complex");
        std::println("--latex | --no-latex");
        std::println("  Choose, whether to print the output in .tex syntax");
        std::println("--x <x1> <x2>");
        std::println("  Choose x bounds of the save file. x1 <= x < x2");
        std::println("--y <y1> <y2>");
        std::println("  Choose y bounds of the save file. y1 <= y < y2");
        std::println("--z <z1> <z2>");
        std::println("  Choose z bounds of the save file. z1 <= z < z2");
        std::println("<path-to-region-directory");
        std::println("Path to the region directory of a minecraft save.");
        return 0;
    }
    auto parser = std::make_unique<MinecraftSavefileParser_mcSavefileParsers>();
    MinecraftCoordinates lower_corner = {
        .x = m_options->x_bounds().first,
        .y = m_options->y_bounds().first,
        .z = m_options->z_bounds().first,
    };
    MinecraftCoordinates upper_corner = {
        .x = m_options->x_bounds().second,
        .y = m_options->y_bounds().second,
        .z = m_options->z_bounds().second,
    };
    auto complex =
        parser->parse(m_options->filename(), lower_corner, upper_corner);
    std::unique_ptr<Homology> homology;
    switch (m_options->homology_to_compute()) {
        case HomologyChoice::Z: {
            homology = complex->z_homology();
            break;
        }
        case HomologyChoice::Z2: {
            homology = complex->z2_homology();
            break;
        }
        case HomologyChoice::Z3: {
            homology = complex->z3_homology();
            break;
        }
    }
    std::unique_ptr<HomologyPrintingStrategy> printing_strategy;
    if (m_options->latex()) {
        switch (m_options->homology_to_compute()) {
            case HomologyChoice::Z: {
                printing_strategy =
                    std::make_unique<HomologyLatexPrint>("\\mathbb{Z}");
                break;
            }
            case HomologyChoice::Z2: {
                printing_strategy =
                    std::make_unique<HomologyLatexPrint>("\\mathbb{Z}_{2}");
                break;
            }
            case HomologyChoice::Z3: {
                printing_strategy =
                    std::make_unique<HomologyLatexPrint>("\\mathbb{Z}_{3}");
                break;
            }
        }
    } else {
        printing_strategy = std::make_unique<HomologyRawPrint>();
    }
    homology->select_strategy(std::move(printing_strategy));
    std::unique_ptr<TextDrawable> output = std::move(homology);
    if (m_options->latex()) {
        output = std::make_unique<LatexWrapper>(std::move(output));
    }
    std::print("{}", output->text());
    return 0;
}

} // namespace core
