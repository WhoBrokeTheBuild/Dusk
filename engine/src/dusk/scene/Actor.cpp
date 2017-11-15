#include "dusk/scene/Actor.hpp"

#include <dusk/core/Log.hpp>
#include <dusk/core/Benchmark.hpp>
#include <dusk/scene/Scene.hpp>

namespace dusk {

Actor::Actor(Actor * parent /*= nullptr*/)
    : _parent(parent)
    , _transform(1)
    , _position(0)
    , _rotation(0)
    , _scale(1)
{
}

void Actor::SetPosition(const glm::vec3& pos)
{
    _position = pos;
}

void Actor::SetRotation(const glm::vec3& rot)
{
    _rotation = rot;
}

void Actor::SetScale(const glm::vec3& scale)
{
    _scale = scale;
}

glm::mat4 Actor::GetTransform()
{
    _transform = (_parent ? _parent->GetTransform() : glm::mat4());
    _transform = glm::translate(_transform, _position);
    _transform = glm::rotate(_transform, _rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    _transform = glm::rotate(_transform, _rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    _transform = glm::rotate(_transform, _rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    _transform = glm::scale(_transform, _scale);

    return _transform;
}

void Actor::Update(const UpdateContext& ctx)
{
    const std::vector<Actor *>& children = GetChildren();
    for (auto& c : children) {
        c->Update(ctx);
    }
}

void Actor::Render(RenderContext& ctx)
{
    const std::vector<Actor *>& children = GetChildren();
    for (auto& c : children) {
        c->Render(ctx);
    }
}

void Actor::RemoveChild(Actor * actor)
{
    auto it = std::find_if(_childrenById.begin(), _childrenById.end(),
        [=](const auto& ptr) {
            return (ptr.second.get() == actor);
        }
    );
    if (it != _childrenById.end()) {
        auto it2 = std::find(_children.begin(), _children.end(), actor);
        _children.erase(it2);

        auto it3 = _tagsByChild.find(actor);
        if (it3 != _tagsByChild.end()) {
            for (auto t : it3->second) {
                auto it4 = std::find(_childrenByTag[t].begin(), _childrenByTag[t].end(), actor);
                _childrenByTag[t].erase(it4);
            }
            _tagsByChild.erase(it3);
        }

        auto it5 = _typesByChild.find(actor);
        _childrenByType.erase(_childrenByType.find(it5->second));
        _typesByChild.erase(it5);

        _childrenById.erase(it);
    }

}

void Actor::RemoveChild(const std::string& id)
{
    auto it = _childrenById.find(id);
    if (it != _childrenById.end()) {
        RemoveChild(it->second.get());
    }
}

void Actor::ChangeChildId(const std::string& oldId, const std::string& newId)
{

}

void Actor::AddChildTag(Actor * actor, const std::string& tag)
{

}

void Actor::AddChildTag(const std::string& id, const std::string& tag)
{

}

void Actor::RemoveChildTag(Actor * actor, const std::string& tag)
{

}

void Actor::RemoveChildTag(const std::string& id, const std::string& tag)
{

}

Actor * Actor::GetChild(const std::string& id)
{
    if (_childrenById.find(id) == _childrenById.end()) {
        return nullptr;
    }
    return _childrenById[id].get();
}

Actor * Actor::GetFirstChild()
{
    if (_children.empty()) {
        return nullptr;
    }
    return _children.front();
}

std::vector<Actor *> Actor::GetChildren()
{
    return _children;
}

Actor * Actor::GetFirstChildWithTag(const std::string& tag)
{
    if (_childrenByTag.find(tag) == _childrenByTag.end()) {
        return nullptr;
    }
    if (_childrenByTag[tag].empty()) {
        return nullptr;
    }
    return _childrenByTag[tag].front();
}

std::vector<Actor *> Actor::GetChildrenWithTag(const std::string& tag)
{
    if (_childrenByTag.find(tag) == _childrenByTag.end()) {
        return std::vector<Actor *>();
    }
    return _childrenByTag[tag];
}

} // namespace dusk
