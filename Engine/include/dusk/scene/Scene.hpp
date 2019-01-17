#ifndef DUSK_SCENE_HPP
#define DUSK_SCENE_HPP

#include <dusk/core/Context.hpp>
#include <dusk/core/Macros.hpp>
#include <dusk/scene/Actor.hpp>

#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace dusk {

class Scene
{
public:

    /// Class Boilerplate

    DISALLOW_COPY_AND_ASSIGN(Scene)

    Scene() = default;
    
    virtual ~Scene() = default;

    /// Methods

    bool LoadFromFile(const std::string& filename);

    virtual void Start();

    virtual void Stop();

    void AddActor(std::unique_ptr<Actor>&& actor);

    virtual void HandleEvent(SDL_Event * evt);

    virtual void Update(UpdateContext& ctx);

    virtual void Render(RenderContext& ctx);

    virtual void Print();

private:

    std::vector<std::unique_ptr<Actor>> _actors;

public:

}; // class Scene

} // namespace dusk

#endif // DUSK_SCENE_HPP
