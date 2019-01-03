#ifndef DUSK_SCENE_HPP
#define DUSK_SCENE_HPP

#include <dusk/Config.hpp>
#include <dusk/core/Context.hpp>
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

    DISALLOW_COPY_AND_ASSIGN(Scene)

    Scene();
    virtual ~Scene() = default;

    virtual void Start();
    virtual void Stop();

    virtual void Update(UpdateContext& ctx);

    virtual void Render(RenderContext& ctx);

private:

    std::vector<std::unique_ptr<Actor>> _actors;

public:

}; // class Scene

} // namespace dusk

#endif // DUSK_SCENE_HPP
