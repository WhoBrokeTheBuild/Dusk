#ifndef DUSK_SPAN_HPP
#define DUSK_SPAN_HPP

#include <span>
#include <algorithm>

namespace dusk {

template <typename T, size_t Extent = std::dynamic_extent>
using Span = std::span<T, Extent>;

template <typename T>
inline bool SpanContains(const Span<T>& haystack, const T& needle) {
    return (std::find(haystack.begin(), haystack.end(), needle) != haystack.end());
}

} // namespace dusk

#endif // DUSK_SPAN_HPP