#ifndef DUSK_ENUMERATE_HPP
#define DUSK_ENUMERATE_HPP

#include <Dusk/Macros.hpp>

#include <algorithm>

namespace dusk {

template <typename Iterable>
class Enumerator
{
public:

    Enumerator(Iterable iterable)
        : _index(0)
        , _iterable(iterable)
        , _iterator(std::begin(iterable))
        , _end(std::end(iterable))
    { }

    inline const Enumerator& begin() const {
        return *this;
    }

    inline const Enumerator& end() const {
        return *this;
    }

    inline bool operator!=(const Enumerator&) const {
        return _iterator != _end;
    }

    inline void operator++() {
        ++_iterator;
        ++_index;
    }

    inline auto operator*() const {
        return std::pair(_index, *_iterator);
    }

private:

    std::size_t _index;

    Iterable _iterable;

    decltype(std::begin(_iterable)) _iterator;

    const decltype(std::end(_iterable)) _end;

};

template <typename Iterable>
inline auto Enumerate(Iterable&& iterable) {
    return Enumerator<Iterable>(std::forward<Iterable>(iterable));
}

} // namespace dusk

#endif // DUSK_ENUMERATE_HPP