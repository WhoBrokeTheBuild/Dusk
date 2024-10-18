#ifndef DUSK_SET_HPP
#define DUSK_SET_HPP

#include <set>
#include <algorithm>

namespace dusk {
    
template <typename T>
using Set = std::set<T>;

template <typename T>
inline bool SetContains(const Set<T>& haystack, const T& needle) {
    return (std::find(haystack.begin(), haystack.end(), needle) != haystack.end());
}

} // namespace dusk

#endif // DUSK_SET_HPP