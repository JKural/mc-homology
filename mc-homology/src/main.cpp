#include <memory>
#include <print>

#include "core/manager.h"
#include "core/options.h"

int main(int argc, char** argv) {
    std::unique_ptr<core::MinecraftSavefileParser> parser =
        std::make_unique<core::MinecraftSavefileParser_mcSavefileParsers>();
    std::unique_ptr<core::Options> options = nullptr;
    try {
        options = std::make_unique<core::CommandlineOptions>(argc, argv);
    } catch (std::invalid_argument& e) {
        std::println("Error when parsing options: {}", e.what());
        return 1;
    }
    core::Manager manager(std::move(options), std::move(parser));
    return manager.run();
}
