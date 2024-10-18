#ifndef DUSK_MAP_HPP
#define DUSK_MAP_HPP

#include <unordered_map>

namespace dusk {

template <typename K, typename T, typename Hash = std::hash<T>>
using Map = std::unordered_map<K, T>;

} // namespace dusk

#endif // DUSK_MAP_HPP