#ifndef DUSK_LIST_HPP
#define DUSK_LIST_HPP

#include <vector>
#include <algorithm>

namespace dusk {

template <typename T>
using List = std::vector<T>;

template <typename T>
inline bool ListContains(const List<T>& haystack, const T& needle) {
    return (std::find(haystack.begin(), haystack.end(), needle) != haystack.end());
}

template <typename T>
inline bool ListRemove(List<T>& haystack, const T& needle) {
    auto it = std::find(haystack.begin(), haystack.end(), needle);
    if (it == haystack.end()) {
        return false;
    }

    haystack.erase(it);
    return true;
}

} // namespace dusk

#endif // DUSK_LIST_HPP