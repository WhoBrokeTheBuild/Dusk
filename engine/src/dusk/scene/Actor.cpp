#include "dusk/scene/Actor.hpp"

#include <dusk/core/Log.hpp>
#include <dusk/core/Benchmark.hpp>
#include <dusk/scene/Scene.hpp>

namespace dusk {

Actor::Actor()
    : _parent(nullptr)
    , _id()
    , _transform(1)
    , _position(0)
    , _rotation(0)
    , _scale(1)
{
}

void Actor::Serialize(nlohmann::json& data)
{
    // Children

    glm::vec3 position = GetPosition();
    data["Position"] = { position.x, position.y, position.z };

    glm::vec3 rotation = GetRotation();
    data["Rotation"] = { rotation.x, rotation.y, rotation.z };

    glm::vec3 scale = GetScale();
    data["Scale"] = { scale.x, scale.y, scale.z };
}

void Actor::Deserialize(nlohmann::json& data)
{
    DuskLogInfo("Deserializinng Actor");

    if (data.find("Children") != data.end()) {
        for (auto& actor : data["Children"]) {
            std::string id = actor["ID"].get<std::string>();
            std::string type = actor["Type"].get<std::string>();
            DuskLogInfo("Creating Actor of Type '%s'", type.c_str());

            std::unique_ptr<BaseClass> base = std::unique_ptr<BaseClass>(BaseClass::Initializers[type]());
            BaseClass::Deserializers[type](base.get(), actor);

            std::unique_ptr<Actor> tmp(dynamic_cast<Actor*>(base.release()));
            AddChild<Actor>(std::move(tmp), id);
        }
    }

    if (data.find("Position") != data.end()) {
        glm::vec3 position = { data["Position"][0], data["Position"][1], data["Position"][2] };
        SetPosition(position);
    }

    if (data.find("Rotation") != data.end()) {
        glm::vec3 rotation = { data["Rotation"][0], data["Rotation"][1], data["Rotation"][2] };
        SetRotation(rotation);
    }

    if (data.find("Scale") != data.end()) {
        glm::vec3 scale = { data["Scale"][0], data["Scale"][1], data["Scale"][2] };
        SetScale(scale);
    }
}

void Actor::SetId(const std::string& id)
{
    _id = id;
}

void Actor::SetParent(Actor * parent)
{
    _parent = parent;
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

std::type_index Actor::GetChildType(const std::string& id) const
{
    if (_childrenById.find(id) == _childrenById.end()) {
        return typeid(nullptr);
    }
    return _typesByChild.at(_childrenById.at(id).get());
}

std::type_index Actor::GetChildType(Actor * actor) const
{
    if (_typesByChild.find(actor) == _typesByChild.end()) {
        return typeid(nullptr);
    }
    return _typesByChild.at(actor);
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
