#ifndef DUSK_TUPLE_HPP
#define DUSK_TUPLE_HPP

#include <tuple>

namespace dusk {
    
template <class... T>
using Tuple = std::tuple<T...>;

} // namespace dusk

#endif // DUSK_TUPLE_HPP