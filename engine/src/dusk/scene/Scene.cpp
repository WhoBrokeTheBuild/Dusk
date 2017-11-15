#include "dusk/scene/Scene.hpp"

#include <dusk/core/App.hpp>
#include <dusk/core/Log.hpp>
#include <dusk/core/Benchmark.hpp>

namespace dusk {

Scene::Scene()
    : ITaggedCollection<Actor>()
{
}

void Scene::Update(const UpdateContext& ctx)
{
    const std::vector<Actor *>& children = GetChildren();
    for (auto& c : children) {
        c->Update(ctx);
    }
}

void Scene::Render(RenderContext& ctx)
{
    const std::vector<Actor *>& children = GetChildren();
    for (auto& c : children) {
        c->Render(ctx);
    }
}

} // namespace dusk
