#ifndef DUSK_ENTITY_HPP
#define DUSK_ENTITY_HPP

#include <Dusk/Config.hpp>
#include <Dusk/Object.hpp>
#include <Dusk/Math.hpp>
#include <Dusk/Component.hpp>
#include <Dusk/UpdateContext.hpp>
#include <Dusk/RenderContext.hpp>
#include <Dusk/String.hpp>

#include <vector>
#include <memory>

namespace Dusk {

DUSK_ENGINE_API
constexpr vec3 GetWorldUp() {
    return { 0.f, -1.f, 0.f };
}

DUSK_ENGINE_API 
constexpr vec3 GetWorldForward() {
    return { 0.f, 0.f, -1.f };
}


class DUSK_ENGINE_API Entity : public Object
{
public:

    DISALLOW_COPY_AND_ASSIGN(Entity)

    Entity() = default;

    virtual ~Entity() = default;

    bool LoadFromFile(const string& filename);

    void SetParent(Entity * parent);

    inline Entity * GetParent() const {
        return _parent;
    }

    Entity * AddChild(std::unique_ptr<Entity> && child);

    std::vector<Entity *> GetChildren() const;

    Component * AddComponent(std::unique_ptr<Component> && component);

    std::vector<Component *> GetComponents() const;

    void SetName(const string& name);

    inline string GetName() const {
        return _name;
    }

    void SetPosition(const vec3& position);

    inline vec3 GetPosition() const {
        return _position;
    }

    void SetOrientation(const quat& orientation);

    inline quat GetOrientation() const {
        return _orientation;
    }

    void SetScale(const vec3& scale);

    inline vec3 GetScale() const {
        return _scale;
    }

    mat4 GetTransform() const;

    vec3 GetWorldPosition() const;
    
    quat GetWorldOrientation() const;
    
    vec3 GetWorldScale() const;

    mat4 GetWorldTransform() const;

    void Update(UpdateContext * ctx);

    void Render(RenderContext * ctx);

private:

    Entity * _parent = nullptr;

    string _name;

    std::vector<std::unique_ptr<Entity>> _children;

    std::vector<Entity *> _childPtrs;

    std::vector<std::unique_ptr<Component>> _components;

    std::vector<Component *> _componentPtrs;

    vec3 _position = vec3(0.f);

    quat _orientation = quat(1.f, 0.f, 0.f, 0.f);

    vec3 _scale = vec3(1.f);

};

} // namespace Dusk

#endif // DUSK_ENTITY_HPP
