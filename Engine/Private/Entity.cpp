#include <Dusk/Entity.hpp>

#include <Dusk/SceneImporter.hpp>

namespace Dusk {

DUSK_ENGINE_API
bool Entity::LoadFromFile(const string& filename)
{
    const auto& importers = GetAllSceneImporters();
    for (const auto& importer : importers) {
        if (importer->LoadFromFile(this, filename)) {
            return true;
        }
    }
    return false;
}

DUSK_ENGINE_API
void Entity::SetParent(Entity * parent)
{
    _parent = parent;
}

DUSK_ENGINE_API
Entity * Entity::AddChild(std::unique_ptr<Entity> && child)
{
    child->SetParent(this);
    _childPtrs.push_back(child.get());
    _children.push_back(std::move(child));

    return _childPtrs.back();
}

DUSK_ENGINE_API
std::vector<Entity *> Entity::GetChildren() const
{
    return _childPtrs;
}

DUSK_ENGINE_API
Component * Entity::AddComponent(std::unique_ptr<Component> && component)
{
    component->Attach(this);
    _componentPtrs.push_back(component.get());
    _components.push_back(std::move(component));

    return _componentPtrs.back();
}

DUSK_ENGINE_API
std::vector<Component *> Entity::GetComponents() const
{
    return _componentPtrs;
}

DUSK_ENGINE_API
void Entity::SetName(const string& name)
{
    _name = name;
}

DUSK_ENGINE_API
void Entity::SetPosition(const vec3& position)
{
    _position = position;
}

DUSK_ENGINE_API
void Entity::SetOrientation(const quat& orientation)
{
    _orientation = orientation;
}

DUSK_ENGINE_API
void Entity::SetScale(const vec3& scale)
{
    _scale = scale;
}

DUSK_ENGINE_API
mat4 Entity::GetTransform() const
{
    mat4 transform = mat4(1.0f);
    transform = glm::translate(transform, GetPosition());
    transform *= glm::mat4_cast(GetOrientation());
    transform = glm::scale(transform, GetScale());
    return transform;
}

DUSK_ENGINE_API
vec3 Entity::GetWorldPosition() const
{
    if (_parent) {
        return _parent->GetWorldPosition() + GetPosition();
    }
    return GetPosition();
}

DUSK_ENGINE_API
quat Entity::GetWorldOrientation() const
{
    if (_parent) {
        return _parent->GetWorldOrientation() * GetOrientation();
    }
    return GetOrientation();
}

DUSK_ENGINE_API
vec3 Entity::GetWorldScale() const
{
    if (_parent) {
        return _parent->GetWorldScale() * GetScale();
    }
    return GetScale();
}

DUSK_ENGINE_API
mat4 Entity::GetWorldTransform() const
{
    if (_parent) {
        return _parent->GetWorldTransform() * GetTransform();
    }
    return GetTransform();
}

DUSK_ENGINE_API
void Entity::Update(UpdateContext * ctx)
{
    for (auto& comp : _components) {
        comp->Update(ctx);
    }

    for (auto& child : _children) {
        child->Update(ctx);
    }
}

DUSK_ENGINE_API
void Entity::Render(RenderContext * ctx)
{
    for (auto& comp : _components) {
        comp->Render(ctx);
    }
    
    for (auto& child : _children) {
        child->Render(ctx);
    }
}

} // namespace Dusk