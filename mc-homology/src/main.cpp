#include <memory>

#include "core/manager.h"
#include "core/options.h"

int main(int argc, char** argv) {
    auto options = std::make_unique<core::CommandlineOptions>(argc, argv);
    core::Manager manager(std::move(options));
    return manager.run();
}
