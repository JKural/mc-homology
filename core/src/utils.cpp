#include "utils.h"

namespace core {
namespace utils {

std::size_t combine_hashes(std::size_t hash1, std::size_t hash2) {
    return hash1 ^ (hash2 << 1);
}

} // namespace utils
} // namespace core
