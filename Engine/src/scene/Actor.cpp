#include "dusk/scene/Actor.hpp"

#include <dusk/core/Benchmark.hpp>
#include <dusk/core/Log.hpp>
#include <dusk/scene/Scene.hpp>

namespace dusk {

void Actor::SetScene(Scene * scene) 
{
    _scene = scene;

    for (auto& actor : _actors) {
        actor->SetScene(_scene);
    }
}

void Actor::SetParent(Actor * actor) 
{
    _parent = actor;
}

void Actor::SetPosition(const glm::vec3& pos)
{
    _position = pos;
}

void Actor::SetRotation(const glm::quat& rot)
{
    _rotation = rot;
}

glm::quat Actor::GetTotalRotation() const
{
    glm::quat rotation = _rotation;
    if (GetParent()) {
        rotation = GetParent()->GetRotation() * rotation;
    }
    return rotation;
}

void Actor::SetScale(const glm::vec3& scale)
{
    _scale = scale;
}

glm::mat4 Actor::GetTransform()
{
    _transform = glm::mat4(1.0f);
    if (GetParent()) {
        _transform = GetParent()->GetTransform();
    }

    _transform = glm::translate(_transform, _position);
    _transform *= glm::mat4_cast(_rotation);
    _transform = glm::scale(_transform, _scale);

    return _transform;
}

void Actor::AddComponent(std::unique_ptr<ActorComponent>&& comp) 
{
    comp->SetActor(this);
    _components.push_back(std::move(comp));
}

void Actor::AddActor(std::unique_ptr<Actor>&& actor)
{
    actor->SetParent(this);
    _actors.push_back(std::move(actor));
}

void Actor::HandleEvent(SDL_Event * evt)
{
    for (auto& comp : _components) {
        comp->HandleEvent(evt);
    }

    for (auto& actor : _actors) {
        actor->HandleEvent(evt);
    }
}

void Actor::Update(UpdateContext& ctx)
{
    for (auto& comp : _components) {
        comp->Update(ctx);
    }

    for (auto& actor : _actors) {
        actor->Update(ctx);
    }
}

void Actor::Render(RenderContext& ctx)
{
    for (auto& comp : _components) {
        comp->Render(ctx);
    }

    for (auto& actor : _actors) {
        actor->Render(ctx);
    }
}

void Actor::Print(std::string indent) 
{
    DuskLog("%s this = %p", indent, this);
    DuskLog("%s _position = %s", indent, glm::to_string(_position));
    DuskLog("%s _rotation = %s", indent, glm::to_string(_rotation));
    DuskLog("%s _scale = %s", indent, glm::to_string(_scale));
    DuskLog("%s _components.size = %zu", indent, _components.size());
    DuskLog("%s _components {", indent);
    for (auto& comp : _components) {
        comp->Print(indent + "  ");
    }
    DuskLog("%s }", indent);
    DuskLog("%s _actors.size = %zu", indent, _actors.size());
    DuskLog("%s _actors {", indent);
    for (auto& actor : _actors) {
        actor->Print(indent + "  ");
    }
    DuskLog("%s }", indent);
}

} // namespace dusk
