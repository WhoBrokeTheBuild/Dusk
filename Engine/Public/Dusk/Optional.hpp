#ifndef DUSK_OPTIONAL_HPP
#define DUSK_OPTIONAL_HPP

#include <optional>

namespace dusk {

template <typename T>
using Optional = std::optional<T>;

using std::nullopt;

} // namespace dusk

#endif // DUSK_OPTIONAL_HPP