/// \file utils.h
/// \brief Contains auxillary classes and functions

#include <concepts>
#include <ranges>

namespace complexes {
namespace utils {

/// \brief Combines two hashes into a single hash
std::size_t combine_hashes(std::size_t hash1, std::size_t hash2);

/// \brief Combines hashes of a range into a single hash
template<std::ranges::range R>
    requires requires(std::ranges::range_value_t<R> x) {
        {
            std::hash<std::ranges::range_value_t<R>> {}(x)
        } -> std::convertible_to<std::size_t>;
    }
std::size_t hash_range(R&& r) {
    std::size_t hash = std::hash<std::size_t> {}(0);
    for (auto&& x : std::forward<R>(r)) {
        hash = combine_hashes(
            hash,
            std::hash<std::ranges::range_value_t<R>> {}(
                std::forward<decltype(x)>(x)
            )
        );
    }
    return hash;
}

} // namespace utils
} // namespace complexes
