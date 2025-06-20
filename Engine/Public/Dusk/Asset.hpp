#ifndef DUSK_ASSET_HPP
#define DUSK_ASSET_HPP

#include <Dusk/Macros.hpp>
#include <Dusk/NonCopyable.hpp>
#include <Dusk/JSON.hpp>

namespace dusk {

class DUSK_API Asset : public NonCopyable
{
public:

    Asset() = default;

    virtual ~Asset() = default;

    virtual bool IsLoaded() const {
        return _isLoaded;
    }

    virtual void SetLoaded(bool isLoaded) {
        _isLoaded = isLoaded;
    }

    virtual bool CanReload() const = 0;

    virtual bool Reload() = 0;

    virtual bool Serialize(JSON::Object& data) const = 0;

    virtual bool Deserialize(const JSON::Object& data) = 0;

private:

    bool _isLoaded = false;

}; // class Asset

} // namespace dusk

#endif // DUSK_ASSET_HPP