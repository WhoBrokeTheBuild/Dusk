#include "dusk/scene/Scene.hpp"

#include <fstream>
#include <dusk/core/Log.hpp>
#include <dusk/core/Benchmark.hpp>

namespace dusk {

Scene::Scene()
    : Actor()
{
}

bool Scene::Load(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file) {
        return false;
    }

    nlohmann::json data;
    file >> data;

    Deserialize(data);

    return true;
}

bool Scene::Save(const std::string& filename)
{
    std::ofstream file(filename);
    if (!file) {
        return false;
    }

    nlohmann::json data;
    Serialize(data);

    file << data;

    return true;
}

void Scene::Update(UpdateContext& ctx)
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
