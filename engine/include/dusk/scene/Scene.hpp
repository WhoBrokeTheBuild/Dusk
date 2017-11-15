#ifndef DUSK_SCENE_HPP
#define DUSK_SCENE_HPP

#include <dusk/Config.hpp>

#include <dusk/scene/ITaggedCollection.hpp>
#include <dusk/core/Context.hpp>
#include <dusk/scene/Actor.hpp>
#include <dusk/scene/Camera.hpp>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace dusk {

class Scene : public ITaggedCollection<Actor>
{
public:

    DISALLOW_COPY_AND_ASSIGN(Scene);

    Scene();
    virtual ~Scene() = default;

    void Update(const UpdateContext& ctx);
    void Render(RenderContext& ctx);

private:

}; // class Scene

} // namespace dusk

#endif // DUSK_SCENE_HPP
