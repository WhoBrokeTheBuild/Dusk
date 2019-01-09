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

glm::vec3 Actor::GetWorldPosition() const
{
    if (GetParent()) {
        return GetParent()->GetWorldPosition() + GetPosition();
    }
    return GetPosition();
}

void Actor::SetRotation(const glm::quat& rot)
{
    _rotation = rot;
}

glm::quat Actor::GetWorldRotation() const
{
    if (GetParent()) {
        return GetParent()->GetWorldRotation() * GetRotation();
    }
    return GetRotation();
}

void Actor::SetScale(const glm::vec3& scale)
{
    _scale = scale;
}

glm::vec3 Actor::GetWorldScale() const {
    if (GetParent()) {
        return GetParent()->GetWorldScale() * GetScale();
    }
    return GetScale();
}

glm::mat4 Actor::GetTransform() const
{
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, _position);
    transform *= glm::mat4_cast(_rotation);
    transform = glm::scale(transform, _scale);

    return transform;
}

glm::mat4 Actor::GetWorldTransform() const
{
    if (GetParent()) {
        return GetParent()->GetWorldTransform() * GetTransform();
    }
    return GetTransform();
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

void Actor::SceneStart()
{
    for (auto& comp : _components) {
        comp->SceneStart();
    }

    for (auto& actor : _actors) {
        actor->SceneStart();
    }
}

void Actor::SceneStop()
{
    for (auto& comp : _components) {
        comp->SceneStop();
    }

    for (auto& actor : _actors) {
        actor->SceneStop();
    }
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
