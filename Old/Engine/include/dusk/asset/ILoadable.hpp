#ifndef DUSK_ILOADABLE_HPP
#define DUSK_ILOADABLE_HPP

#include <algorithm>

namespace dusk {

class ILoadable 
{
public:

    ILoadable() = default;

    virtual ~ILoadable() = default;

    ILoadable(ILoadable&& rhs) {
        std::swap(_loaded, rhs._loaded);
    }

    virtual bool IsLoaded() const {
        return _loaded;
    }

    virtual void SetLoaded(bool loaded) {
        _loaded = loaded;
    }

private:

    bool _loaded = false;

}; // class ILoadable

} // namespace dusk

#endif // DUSK_ILOADABLE_HPP
