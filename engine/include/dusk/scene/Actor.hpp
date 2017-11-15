#ifndef DUSK_ACTOR_HPP
#define DUSK_ACTOR_HPP

#include <dusk/Config.hpp>

#include <dusk/scene/ITaggedCollection.hpp>
#include <dusk/core/Context.hpp>

#include <memory>

namespace dusk {

class Actor : public ITaggedCollection<Actor>
{
public:

    DISALLOW_COPY_AND_ASSIGN(Actor);

    Actor(std::string name, Actor * parent = nullptr);
    virtual ~Actor() = default;

    std::string GetName() { return _name; }

    inline Actor * GetParent() { return _parent; }

    void SetPosition(const glm::vec3& pos);
    inline glm::vec3 GetPosition() const { return _position; }

    void SetRotation(const glm::vec3& rot);
    inline glm::vec3 GetRotation() const { return _rotation; }

    void SetScale(const glm::vec3& scale);
    inline glm::vec3 GetScale() const { return _scale; }

    glm::mat4 GetTransform();

    virtual void Update(const UpdateContext& ctx);
    virtual void Render(RenderContext& ctx);

private:

    std::string _name;

    Actor * _parent;

    glm::mat4 _transform;
    glm::vec3 _position;
    glm::vec3 _rotation;
    glm::vec3 _scale;

}; // class Actor

} // namespace dusk

#endif // DUSK_ACTOR_HPP
