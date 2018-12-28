#ifndef DUSK_SCENE_HPP
#define DUSK_SCENE_HPP

#include <dusk/Config.hpp>

#include <dusk/scene/Actor.hpp>
#include <dusk/core/Context.hpp>

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <unordered_map>
using std::unordered_map;

#include <type_traits>
using std::is_base_of;

#include <memory>
using std::unique_ptr;

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

    vector<unique_ptr<Actor>> _actors;

public:

}; // class Scene

} // namespace dusk

#endif // DUSK_SCENE_HPP
