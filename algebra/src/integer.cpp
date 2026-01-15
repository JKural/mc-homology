#include "integer.h"

namespace algebra {

std::ostream& operator<<(std::ostream& output, Integer k) {
    return output << static_cast<int>(k);
}

} // namespace algebra
