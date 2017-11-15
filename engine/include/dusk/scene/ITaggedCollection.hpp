#ifndef DUSK_ITAGGED_COLLECTION_HPP
#define DUSK_ITAGGED_COLLECTION_HPP

#include <dusk/Config.hpp>
#include <dusk/core/Log.hpp>
#include <memory>
#include <vector>
#include <unordered_map>

namespace dusk {

template <class T>
class ITaggedCollection
{
public:

    ITaggedCollection()
        : _children()
        , _childrenByTag()
    {
    }

    template <class K = T>
    K * AddChild(std::unique_ptr<K>&& child)
    {
        K * tmp = child.get();

        if (T * conv = dynamic_cast<T *>(child.get())) {
            child.release();
            _childrenRaw.push_back(conv);
            _children.push_back(std::unique_ptr<T>(conv));
            return tmp;
        }

        return nullptr;
    }

    bool RemoveChild(T * child)
    {
        auto it = std::find_if(_children.begin(), _children.end(),
            [=](const std::unique_ptr<T>& ptr) -> bool {
                return child == ptr.get();
            }
        );

        if (it == _children.end()) {
            return false;
        }

        _children.erase(it);

        auto it2 = std::find(_childrenRaw.begin(), _childrenRaw.end(), child);
        if (it2 != _childrenRaw.end()) {
            _childrenRaw.erase(it2);
        }

        return true;
    }

    void AddTag()
    {

    }

    void RemoveTag()
    {

    }

    std::vector<T *> GetChildren()
    {
        return _childrenRaw;
    }

protected:

    std::vector<std::unique_ptr<T>> _children;
    std::vector<T *> _childrenRaw;

    std::unordered_map<std::string, std::vector<T *>> _childrenByTag;

}; // class ITaggedCollection<T>

} // namespace dusk

#endif // DUSK_SCENE_ITAGGED_COLLECTION_HPP
