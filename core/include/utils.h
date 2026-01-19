/// \file utils.h
/// \brief Contains auxillary classes and functions

#include <string>

namespace core {
namespace utils {

/// \brief Interface for string printable classes
///
/// String printable classes contain a to_string() method, that returns
/// a string representation of the class.
class IStringPrintable {
public:
    virtual std::string to_string() const = 0;
};

/// \brief Combines two hashes into a single hash
std::size_t combine_hashes(std::size_t hash1, std::size_t hash2);

} // namespace utils
} // namespace core
