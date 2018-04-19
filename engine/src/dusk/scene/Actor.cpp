#include "dusk/scene/Actor.hpp"

#include <dusk/core/Log.hpp>
#include <dusk/core/Benchmark.hpp>
#include <dusk/scene/Scene.hpp>

namespace dusk {

Actor::Actor(Scene * scene)
    : _scene(scene)
    , _id()
    , _transform(1)
    , _position(0)
    , _rotation(0)
    , _scale(1)
{
    TrackCallback(scene->OnUpdate.AddPassthrough(&OnUpdate));
    TrackCallback(scene->OnRender.AddPassthrough(&OnRender));
}

void Actor::Serialize(nlohmann::json& data)
{
    glm::vec3 position = GetPosition();
    data["Position"] = { position.x, position.y, position.z };

    glm::vec3 rotation = GetRotation();
    data["Rotation"] = { rotation.x, rotation.y, rotation.z };

    glm::vec3 scale = GetScale();
    data["Scale"] = { scale.x, scale.y, scale.z };
}

void Actor::Deserialize(nlohmann::json& data)
{
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
    //_transform = (_parent ? _parent->GetTransform() : glm::mat4());
    _transform = glm::mat4();
    _transform = glm::translate(_transform, _position);
    _transform = glm::rotate(_transform, _rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    _transform = glm::rotate(_transform, _rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    _transform = glm::rotate(_transform, _rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    _transform = glm::scale(_transform, _scale);

    return _transform;
}

void Actor::AddComponent(std::unique_ptr<IComponent>&& ptr)
{
    _components.push_back(std::move(ptr));
}

void Actor::AddTag(std::string tag, bool propagate /*= true*/)
{
    _tags.push_back(tag);

    if (propagate) {
        _scene->TagActor(this, tag, false);
    }
}

bool Actor::RemoveTag(std::string tag, bool propagate /*= true*/)
{
    auto it = std::find(_tags.begin(), _tags.end(), tag);
    if (it != _tags.end()) {
        _tags.erase(it);
    }

    if (propagate) {
        _scene->UntagActor(this, tag, false);
    }

    return true;
}

} // namespace dusk
