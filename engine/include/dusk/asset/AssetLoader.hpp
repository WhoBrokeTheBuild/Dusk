#ifndef DUSK_ASSET_LOADERE_HPP
#define DUSK_ASSET_LOADERE_HPP

#include <dusk/Config.hpp>
#include <dusk/asset/Asset.hpp>
#include <dusk/asset/Mesh.hpp>

#include <memory>
#include <unordered_map>

namespace dusk {

class AssetLoader
{
public:

    DISALLOW_COPY_AND_ASSIGN(AssetLoader)

    AssetLoader() = delete;

    template <class T>
    static T * Load(const std::string& filename)
    {
        Asset * raw = nullptr;
        std::unique_ptr<Asset> ptr(nullptr);

        auto it = _AssetsByFilename.find(filename);
        if (it != _AssetsByFilename.end()) {
            raw = it->second;
        }
        else {
            ptr.reset(new T(filename));
            raw = ptr.get();

            _Assets[raw] = std::move(ptr);
            _AssetUseCount[raw] = 0;
            _AssetsByFilename[filename] = raw;
        }

        return dynamic_cast<T*>(raw);
    }

    static void Lock(Asset * asset)
    {
        auto it = _AssetUseCount.find(asset);
        if (it != _AssetUseCount.end()) {
            ++it->second;
        }
    }

    static void Release(Asset * asset)
    {
        auto it = _AssetUseCount.find(asset);
        if (it != _AssetUseCount.end()) {
            --it->second;
            if (it->second <= 0) {
                _AssetUseCount.erase(it);
                _Assets.erase(_Assets.find(asset));
            }
        }
    }

    template <class T>
    static T * LoadAndLock(const std::string& filename)
    {
        T * ptr = Load<T>(filename);
        Lock(static_cast<Asset*>(ptr));
        return ptr;
    }

private:

    static std::unordered_map<Asset *, std::unique_ptr<Asset>> _Assets;
    static std::unordered_map<Asset *, int> _AssetUseCount;
    static std::unordered_map<std::string, Asset *> _AssetsByFilename;

}; // class AssetLoader

} // namespace dusk

#endif // DUSK_ASSET_LOADERE_HPP
