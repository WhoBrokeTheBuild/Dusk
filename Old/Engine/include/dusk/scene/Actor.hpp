#ifndef DUSK_ACTOR_HPP
#define DUSK_ACTOR_HPP

#include <dusk/core/Context.hpp>
#include <dusk/core/Macros.hpp>
#include <dusk/core/Math.hpp>
#include <dusk/scene/ActorComponent.hpp>

#include <memory>
#include <string>
#include <vector>

namespace dusk {

class Scene;

class Actor
{
public:

    /// Boilerplate

    DISALLOW_COPY_AND_ASSIGN(Actor)

    Actor() = default;

    virtual ~Actor() = default;

    /// Methods

    void SetScene(Scene *);

    Scene * GetScene() const { 
        return _scene;
    }

    void SetParent(Actor *);

    Actor * GetParent() const {
        return _parent;
    }

    void SetPosition(const glm::vec3&);

    inline glm::vec3 GetPosition() const {
        return _position;
    }

    glm::vec3 GetWorldPosition() const;

    void SetRotation(const glm::quat&);

    inline glm::quat GetRotation() const {
        return _rotation;
    }

    glm::quat GetWorldRotation() const;

    void SetScale(const glm::vec3&);

    inline glm::vec3 GetScale() const { 
        return _scale; 
    }

    glm::vec3 GetWorldScale() const;

    glm::mat4 GetTransform() const;

    glm::mat4 GetWorldTransform() const;

    virtual void AddComponent(std::unique_ptr<ActorComponent>&&);

    virtual void AddActor(std::unique_ptr<Actor>&&);

    virtual void SceneStart();

    virtual void SceneStop();

    virtual void HandleEvent(SDL_Event *);

    virtual void Update(UpdateContext&);
    
    virtual void Render(RenderContext&);

    virtual void Print(std::string indent);

private:

    /// Variables

    Scene * _scene = nullptr;

    Actor * _parent = nullptr;

    glm::vec3 _position = glm::vec3(0.0f);

    glm::quat _rotation = glm::quat(1.0f, 0.f, 0.f, 0.f);
    
    glm::vec3 _scale = glm::vec3(1.0f);

    std::vector<std::unique_ptr<ActorComponent>> _components;

    std::vector<std::unique_ptr<Actor>> _actors;

}; // class Actor

} // namespace dusk

#endif // DUSK_ACTOR_HPP
