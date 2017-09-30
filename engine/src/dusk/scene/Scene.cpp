#include "dusk/scene/Scene.hpp"

#include <dusk/core/App.hpp>
#include <dusk/core/Log.hpp>
#include <dusk/core/Benchmark.hpp>

namespace dusk {

std::unique_ptr<Scene> Scene::Create()
{
    return std::make_unique<Scene>();
}

Scene::Scene()
    : _cameras()
    , _actors()
{ }

Actor * Scene::AddActor(std::unique_ptr<Actor> actor)
{
    _actors.push_back(std::move(actor));
    return _actors.back().get();
}

Camera * Scene::AddCamera(std::unique_ptr<Camera> camera)
{
    _cameras.push_back(std::move(camera));
    return _cameras.back().get();
}

} // namespace dusk
