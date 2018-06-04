#ifndef DUSK_ASSET_HPP
#define DUSK_ASSET_HPP

#include <dusk/Config.hpp>

namespace dusk {

class Asset
{
public:

    Asset(const std::string& filename)
        : _filename(filename)
    { }
    
    inline bool IsLoaded() const { return _loaded; }
    inline std::string GetFilename() const { return _filename; }

protected:

    virtual void Load() = 0;
    virtual void Free() = 0;

    inline void SetLoaded(bool loaded) { _loaded = loaded; }
    inline void SetFilename(const std::string& filename) { _filename = filename; }

private:

    bool _loaded = false;
    std::string _filename;

};

} // namespace dusk

#endif // DUSK_ASSET_HPP
