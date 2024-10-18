#ifndef DUSK_ARRAY_HPP
#define DUSK_ARRAY_HPP

#include <array>
#include <algorithm>

namespace dusk {

template <typename T, std::size_t N>
using Array = std::array<T, N>;

template <typename T, std::size_t N>
inline bool ArrayContains(const Array<T, N>& haystack, const T& needle) {
    return (std::find(std::begin(haystack), std::end(haystack), needle) != std::end(haystack));
}

} // namespace dusk

#endif // DUSK_ARRAY_HPP