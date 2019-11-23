#include "dusk/scene/Scene.hpp"

#include <dusk/core/App.hpp>
#include <dusk/core/Benchmark.hpp>
#include <dusk/core/glTF2.hpp>
#include <dusk/core/Log.hpp>
#include <dusk/core/Util.hpp>
#include <dusk/scene/Axis.hpp>
#include <dusk/scene/Camera.hpp>
#include <dusk/scene/MeshComponent.hpp>
#include <dusk/asset/Mesh.hpp>
#include <dusk/asset/Material.hpp>
#include <dusk/asset/Texture.hpp>

#include <fstream>
#include <functional>

namespace dusk {

bool Scene::LoadFromFile(const std::string& filename)
{
    DuskBenchStart();

    auto actors = glTF2::LoadSceneFromFile(filename);
    for (auto& actor : actors) {
        AddActor(std::move(actor));
    }

    DuskBenchEnd("Scene::LoadFromFile");
    return true;
}

void Scene::Start()
{
    for (auto& actor : _actors) {
        actor->SceneStart();
    }
}

void Scene::Stop()
{
    for (auto& actor : _actors) {
        actor->SceneStop();
    }
}

void Scene::AddActor(std::unique_ptr<Actor>&& actor)
{
    actor->SetScene(this);
    _actors.push_back(std::move(actor));
}

void Scene::HandleEvent(SDL_Event * evt)
{
    for (auto& actor : _actors) {
        actor->HandleEvent(evt);
    }
}

void Scene::Update(UpdateContext& ctx)
{
    for (auto& actor : _actors) {
        actor->Update(ctx);
    }
}

void Scene::Render(RenderContext& ctx)
{
    for (auto& actor : _actors) {
        actor->Render(ctx);
    }
}

void Scene::Print()
{
    DuskLog("_actors.size = %zu", _actors.size());
    DuskLog("_actors {", _actors.size());
    for (auto& actor : _actors) {
        actor->Print("  ");
    }
    DuskLog("}");
}

} // namespace dusk
