#ifndef DUSK_SCENE_HPP
#define DUSK_SCENE_HPP

#include <dusk/Config.hpp>

#include <dusk/core/Context.hpp>
#include <dusk/scene/Actor.hpp>
#include <dusk/scene/Camera.hpp>
#include <string>
#include <vector>
#include <memory>

namespace dusk {

class Scene
{
public:

    DISALLOW_COPY_AND_ASSIGN(Scene);

    static std::unique_ptr<Scene> Create();

    Scene();
    virtual ~Scene() = default;

    Actor * AddActor(std::unique_ptr<Actor> actor);

    Camera * AddCamera(std::unique_ptr<Camera> camera);

private:

    std::vector<std::unique_ptr<Camera>> _cameras;
    std::vector<std::unique_ptr<Actor>> _actors;

}; // class Scene

} // namespace dusk

#endif // DUSK_SCENE_HPP
