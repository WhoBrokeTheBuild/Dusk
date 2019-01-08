#ifndef DUSK_ACTOR_HPP
#define DUSK_ACTOR_HPP

#include <dusk/Config.hpp>

#include <dusk/core/Context.hpp>
#include <dusk/core/Log.hpp>
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

    DISALLOW_COPY_AND_ASSIGN(Actor)

    Actor() = default;

    virtual ~Actor() = default;

    void SetScene(Scene * scene);

    Scene * GetScene() const { 
        return _scene;
    }

    void SetParent(Actor * actor);

    Actor * GetParent() const {
        return _parent;
    }

    void SetPosition(const glm::vec3& pos);

    inline glm::vec3 GetPosition() const {
        return _position;
    }

    glm::vec3 GetWorldPosition() const;

    void SetRotation(const glm::quat& rot);

    inline glm::quat GetRotation() const {
        return _rotation;
    }

    glm::quat GetWorldRotation() const;

    void SetScale(const glm::vec3& scale);

    inline glm::vec3 GetScale() const { 
        return _scale; 
    }

    glm::vec3 GetWorldScale() const;

    glm::mat4 GetTransform() const;

    glm::mat4 GetWorldTransform() const;

    virtual void AddComponent(std::unique_ptr<ActorComponent>&& comp);

    virtual void AddActor(std::unique_ptr<Actor>&& actor);

    virtual void HandleEvent(SDL_Event * evt);

    virtual void Update(UpdateContext& ctx);
    
    virtual void Render(RenderContext& ctx);

    virtual void Print(std::string indent);

private:

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
